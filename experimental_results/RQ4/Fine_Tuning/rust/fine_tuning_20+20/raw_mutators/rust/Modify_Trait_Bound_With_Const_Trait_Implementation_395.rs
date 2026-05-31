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

pub struct Modify_Trait_Bound_With_Const_Trait_Implementation_395;

impl Mutator for Modify_Trait_Bound_With_Const_Trait_Implementation_395 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_With_Const_Trait_Implementation_395"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut found = false;
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        for bound in &type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.modifier == syn::TraitBoundModifier::MaybeConst {
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if !found {
                    continue;
                }
                let mut new_generics = func.sig.generics.clone();
                new_generics.params.clear();
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        let mut new_type_param = type_param.clone();
                        new_type_param.bounds.clear();
                        for bound in &type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.modifier == syn::TraitBoundModifier::MaybeConst {
                                    new_type_param
                                        .bounds
                                        .push(parse_quote!(~const Clone));
                                } else {
                                    new_type_param.bounds.push(bound.clone());
                                }
                            } else {
                                new_type_param.bounds.push(bound.clone());
                            }
                        }
                        new_generics
                            .params
                            .push(syn::GenericParam::Type(new_type_param));
                    } else {
                        new_generics.params.push(param.clone());
                    }
                }
                func.sig.generics = new_generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with const trait bounds, specifically those using the `~const` syntax. It replaces such bounds with `~const Clone`, leveraging the const trait implementation feature. This transformation stresses the compiler's const trait resolution mechanism and can reveal bugs related to const trait inference, bound checking, and code generation."
    }
}