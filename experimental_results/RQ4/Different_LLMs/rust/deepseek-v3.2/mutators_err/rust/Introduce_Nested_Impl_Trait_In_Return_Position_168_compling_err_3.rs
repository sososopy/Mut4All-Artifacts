use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Introduce_Nested_Impl_Trait_In_Return_Position_168;

impl Mutator for Introduce_Nested_Impl_Trait_In_Return_Position_168 {
    fn name(&self) -> &str {
        "Introduce_Nested_Impl_Trait_In_Return_Position_168"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if let Some(first_bound) = type_impl_trait.bounds.first() {
                            if let TypeParamBound::Trait(trait_bound) = first_bound {
                                let trait_path = &trait_bound.path;
                                let trait_name = trait_path.segments.last().unwrap().ident.to_string();
                                let trait_segments = trait_path.segments.clone();
                                let mut trait_collector = TraitCollector::new();
                                trait_collector.visit_file(&file);
                                let available_traits = trait_collector.traits;
                                let mut suitable_assoc_trait = None;
                                for available_trait_name in available_traits.iter() {
                                    if *available_trait_name != trait_name {
                                        suitable_assoc_trait = Some(available_trait_name.clone());
                                        break;
                                    }
                                }
                                if let Some(assoc_trait) = suitable_assoc_trait {
                                    let new_return_type: Type = parse_quote! {
                                        impl #trait_path<Item = impl #assoc_trait>
                                    };
                                    item_fn.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                                    let new_body: Expr = parse_quote! {
                                        Box::new(()) as Box<dyn #trait_path<Item = impl #assoc_trait>>
                                    };
                                    let mut new_block = item_fn.block.clone();
                                    new_block.stmts.clear();
                                    new_block.stmts.push(Stmt::Expr(new_body, None));
                                    item_fn.block = new_block;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that return an impl Trait. It modifies the return type to introduce nested impl Trait within an associated type (like Iterator::Item) and then coerces the returned value to a trait object (dyn Trait) that uses impl Trait in its associated type. Specifically, locate a function with a return type of impl Trait. Change the return type to impl Trait1<Assoc = impl Trait2>, where Assoc is an associated type of Trait1 (commonly Iterator::Item). In the function body, produce a value that implements Trait1 with a concrete type for Assoc, then cast or coerce it (using as) to a dyn Trait1<Assoc = impl Trait2>. Ensure the concrete type for Assoc is something that can be coerced to dyn Trait1 (like Box<Concrete> to Box<dyn Trait1>). Use types and traits already present in the seed program; if Iterator is not available, use another trait with an associated type that exists. For example, if the seed has a function returning impl Iterator, change it to return impl Iterator<Item = impl Display> and in the body write Box::new(some_iter) as Box<dyn Iterator<Item = impl Display>>. If the seed lacks Display, use another appropriate trait from the program. Do not introduce new names beyond the nested impl Trait syntax; reuse existing trait and type names. The change is only applied if the function originally returns an impl Trait and there is a suitable trait with an associated type available in scope."
    }
}

struct TraitCollector {
    traits: HashSet<String>,
}

impl TraitCollector {
    fn new() -> Self {
        TraitCollector {
            traits: HashSet::new(),
        }
    }
}

impl Visit<'_> for TraitCollector {
    fn visit_path(&mut self, path: &syn::Path) {
        if let Some(segment) = path.segments.last() {
            let ident = segment.ident.to_string();
            if ident.chars().next().unwrap().is_uppercase() {
                self.traits.insert(ident);
            }
        }
        syn::visit::visit_path(self, path);
    }
}