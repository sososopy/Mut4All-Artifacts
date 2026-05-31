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
};

use crate::mutator::Mutator;

pub struct Alter_PointerLike_Trait_Usage_33;

impl Mutator for Alter_PointerLike_Trait_Usage_33 {
    fn name(&self) -> &str {
        "Alter_PointerLike_Trait_Usage_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                // Check function parameters for PointerLike trait usage
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            if type_impl_trait.bounds.iter().any(|bound| {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    trait_bound.path.is_ident("PointerLike")
                                } else {
                                    false
                                }
                            }) {
                                // Replace PointerLike with Box<dyn Debug>
                                let new_type: Type = parse_quote!(Box<dyn Debug>);
                                pat_type.ty = Box::new(new_type);
                            }
                        }
                    }
                }
                // Check return type for PointerLike trait usage
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**ty {
                        if type_impl_trait.bounds.iter().any(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.path.is_ident("PointerLike")
                            } else {
                                false
                            }
                        }) {
                            // Replace PointerLike with Box<dyn Debug>
                            let new_type: Type = parse_quote!(Box<dyn Debug>);
                            *ty = Box::new(new_type);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions using the PointerLike trait in parameters or return types. It replaces PointerLike with Box<dyn Debug>, ensuring type correctness and testing the compiler's handling of dynamic trait objects and casting behavior."
    }
}