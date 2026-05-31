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

pub struct Introduce_Higher_Ranked_Trait_Bound_On_Associated_Type_9;

impl Mutator for Introduce_Higher_Ranked_Trait_Bound_On_Associated_Type_9 {
    fn name(&self) -> &str {
        "Introduce_Higher_Ranked_Trait_Bound_On_Associated_Type_9"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    let mut has_assoc_type = false;
                                    for segment in &trait_bound.path.segments {
                                        if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                                            for arg in &args.args {
                                                if let syn::GenericArgument::AssocType(_) = arg {
                                                    has_assoc_type = true;
                                                    break;
                                                }
                                            }
                                            if has_assoc_type {
                                                break;
                                            }
                                        }
                                    }
                                    if has_assoc_type {
                                        let mut lifetimes = syn::BoundLifetimes::default();
                                        lifetimes.lifetimes.push(syn::GenericParam::Lifetime(syn::LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: syn::Lifetime::new("'a", proc_macro2::Span::call_site()),
                                            colon_token: None,
                                        }));
                                        trait_bound.lifetimes = Some(lifetimes);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    for bound in &mut predicate_type.bounds {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            let mut has_assoc_type = false;
                                            for segment in &trait_bound.path.segments {
                                                if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                                                    for arg in &args.args {
                                                        if let syn::GenericArgument::AssocType(_) = arg {
                                                            has_assoc_type = true;
                                                            break;
                                                        }
                                                    }
                                                    if has_assoc_type {
                                                        break;
                                                    }
                                                }
                                            }
                                            if has_assoc_type {
                                                let mut lifetimes = syn::BoundLifetimes::default();
                                                lifetimes.lifetimes.push(syn::GenericParam::Lifetime(syn::LifetimeParam {
                                                    attrs: Vec::new(),
                                                    lifetime: syn::Lifetime::new("'a", proc_macro2::Span::call_site()),
                                                    colon_token: None,
                                                }));
                                                trait_bound.lifetimes = Some(lifetimes);
                                            }
                                        }
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
        "The mutation operator introduces higher-ranked trait bounds (HRTBs) on associated types within function where clauses. It targets trait bounds that reference associated types and wraps them in a for<...> quantifier over a lifetime parameter ('a), enforcing lifetime polymorphism. This transformation tests the compiler's ability to resolve HRTBs in complex trait bounds and may expose bugs in trait resolution or lifetime inference systems."
    }
}