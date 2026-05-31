use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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
    TraitItem,
    TraitItemMethod,
};

use crate::mutator::Mutator;

pub struct Alter_Trait_Method_Return_Types_691;

impl Mutator for Alter_Trait_Method_Return_Types_691 {
    fn name(&self) -> &str {
        "Alter_Trait_Method_Return_Types_691"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Method(method) = item {
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = return_type.as_mut() {
                                let mut rng = thread_rng();
                                let mut bounds: Vec<_> = type_impl_trait.bounds.iter().cloned().collect();
                                match rng.gen_range(0..3) {
                                    0 => { // Add Trait Bound
                                        bounds.push(parse_quote!(Clone));
                                    }
                                    1 => { // Remove Trait Bound
                                        if !bounds.is_empty() {
                                            bounds.remove(rng.gen_range(0..bounds.len()));
                                        }
                                    }
                                    2 => { // Replace Trait Bound
                                        if !bounds.is_empty() {
                                            let index = rng.gen_range(0..bounds.len());
                                            bounds[index] = parse_quote!(Debug);
                                        }
                                    }
                                    _ => {}
                                }
                                type_impl_trait.bounds = Punctuated::from_iter(bounds);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}