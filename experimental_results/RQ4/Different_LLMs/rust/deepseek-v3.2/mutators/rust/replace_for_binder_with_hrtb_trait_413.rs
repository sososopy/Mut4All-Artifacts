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

pub struct Replace_For_Binder_With_HRTB_Trait_413;

impl Mutator for Replace_For_Binder_With_HRTB_Trait_413 {
    fn name(&self) -> &str {
        "Replace_For_Binder_With_HRTB_Trait_413"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_defined = false;
        let mut trait_name = Ident::new("Other", Span::call_site());
        
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.ident == "Other" {
                    trait_defined = true;
                    break;
                }
            }
        }
        
        if !trait_defined {
            let new_trait: Item = parse_quote! {
                trait Other {
                    type Assoc;
                }
            };
            file.items.insert(0, new_trait);
        }
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::BareFn(bare_fn) = &mut *pat_type.ty {
                            let lifetimes_clone = bare_fn.lifetimes.as_ref().map(|bl| bl.lifetimes.clone());
                            let mut found_inherent_assoc = false;
                            let mut assoc_path = None;
                            
                            let mut visitor = AssocTypeVisitor {
                                found: &mut found_inherent_assoc,
                                path: &mut assoc_path,
                            };
                            visitor.visit_type_bare_fn(bare_fn);
                            
                            if found_inherent_assoc {
                                if let Some(path) = assoc_path {
                                    if let Some(lifetimes) = lifetimes_clone {
                                        let new_type: Type = parse_quote! {
                                            impl for<#lifetimes> Other<Assoc = #path>
                                        };
                                        pat_type.ty = Box::new(new_type);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates function signatures containing `for<'a> fn(...)` binders where argument types include inherent associated types from generic structs. It replaces the function binder with a higher-ranked trait bound using a dummy trait (Other) that references the same associated type. If the trait is not defined, it introduces a new empty trait with a generic associated type placeholder. This transformation targets the interaction between higher-ranked binders and inherent associated types, potentially triggering region resolution bugs similar to reported compiler issues."
    }
}

struct AssocTypeVisitor<'a> {
    found: &'a mut bool,
    path: &'a mut Option<syn::Path>,
}

impl<'a> Visit<'a> for AssocTypeVisitor<'a> {
    fn visit_type_path(&mut self, node: &'a syn::TypePath) {
        if node.path.segments.len() > 1 {
            let last_segment = node.path.segments.last().unwrap();
            if last_segment.ident == "Assoc" {
                *self.found = true;
                *self.path = Some(node.path.clone());
            }
        }
        syn::visit::visit_type_path(self, node);
    }
}