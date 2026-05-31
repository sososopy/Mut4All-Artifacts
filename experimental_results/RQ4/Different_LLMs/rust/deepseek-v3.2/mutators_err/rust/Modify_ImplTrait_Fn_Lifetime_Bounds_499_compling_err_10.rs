use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, GenericParam::Lifetime as SynLifetimeParam, Ident,
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

pub struct Modify_ImplTrait_Fn_Lifetime_Bounds_499;

impl Mutator for Modify_ImplTrait_Fn_Lifetime_Bounds_499 {
    fn name(&self) -> &str {
        "Modify_ImplTrait_Fn_Lifetime_Bounds_499"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut available_lifetimes = Vec::new();
                for param in &item_impl.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        available_lifetimes.push(lifetime_param.lifetime.clone());
                    }
                }
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    if let Some(trait_ref) = &item_impl.trait_ {
                        let trait_path = &trait_ref.1;
                        let mut trait_lifetimes = Vec::new();
                        for segment in &trait_path.segments {
                            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                for arg in &args.args {
                                    if let GenericArgument::Lifetime(lifetime) = arg {
                                        trait_lifetimes.push(lifetime.clone());
                                    }
                                }
                            }
                        }
                        available_lifetimes.extend(trait_lifetimes);
                    }
                }
                if available_lifetimes.is_empty() {
                    available_lifetimes.push(Lifetime::new("'static", Span::call_site()));
                    available_lifetimes.push(Lifetime::new("'a", Span::call_site()));
                    available_lifetimes.push(Lifetime::new("'b", Span::call_site()));
                }
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                let mut has_fn_bound = false;
                                for bound in &type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(segment) = trait_bound.path.segments.last() {
                                            if segment.ident == "FnOnce"
                                                || segment.ident == "FnMut"
                                                || segment.ident == "Fn"
                                            {
                                                has_fn_bound = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if has_fn_bound {
                                    let mut method_lifetimes = Vec::new();
                                    for param in &method.sig.generics.params {
                                        if let GenericParam::Lifetime(lifetime_param) = param {
                                            method_lifetimes.push(lifetime_param.lifetime.clone());
                                        }
                                    }
                                    if !method_lifetimes.is_empty() {
                                        let lifetime_to_modify = method_lifetimes.choose(&mut rng);
                                        if let Some(lifetime) = lifetime_to_modify {
                                            let new_bound = available_lifetimes.choose(&mut rng).cloned().unwrap_or_else(|| Lifetime::new("'static", Span::call_site()));
                                            for param in &mut method.sig.generics.params {
                                                if let GenericParam::Lifetime(lifetime_param) = param {
                                                    if lifetime_param.lifetime == *lifetime {
                                                        lifetime_param.bounds.clear();
                                                        lifetime_param.bounds.push(new_bound.clone());
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    } else {
                                        let new_lifetime = available_lifetimes.choose(&mut rng).cloned().unwrap_or_else(|| Lifetime::new("'a", Span::call_site()));
                                        let bound_lifetime = available_lifetimes.choose(&mut rng).cloned().unwrap_or_else(|| new_lifetime.clone());
                                        method.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: new_lifetime.clone(),
                                            colon_token: Some(Default::default()),
                                            bounds: {
                                                let mut bounds = Punctuated::new();
                                                bounds.push(bound_lifetime.clone());
                                                bounds
                                            },
                                        }));
                                        if method.sig.generics.where_clause.is_none() {
                                            method.sig.generics.where_clause = Some(WhereClause {
                                                where_token: Default::default(),
                                                predicates: Punctuated::new(),
                                            });
                                        }
                                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                                            let predicate = WherePredicate::Lifetime(PredicateLifetime {
                                                lifetime: new_lifetime,
                                                colon_token: Default::default(),
                                                bounds: {
                                                    let mut bounds = Punctuated::new();
                                                    bounds.push(bound_lifetime);
                                                    bounds
                                                },
                                            });
                                            where_clause.predicates.push(predicate);
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
        "The mutation operator targets trait implementations where methods return `impl Trait` combined with Fn-family traits (FnOnce, FnMut, Fn). It modifies lifetime bounds in the method signature: either altering existing lifetime parameter bounds to different available lifetimes, or introducing new lifetime parameters with trivial bounds when none exist. This creates edge cases in lifetime handling with unboxed closures, potentially causing lifetime mismatch errors, inference failures, or ICEs in the compiler's trait resolution system."
    }
}