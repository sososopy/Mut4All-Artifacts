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

pub struct Add_Recursive_HRTB_NestedGAT_132;

impl Mutator for Add_Recursive_HRTB_NestedGAT_132 {
    fn name(&self) -> &str {
        "Add_Recursive_HRTB_NestedGAT_132"
    }
    fn mutate(&self, file: &mut syn::File) {
        let nested_trait = parse_quote! {
            trait NestedTrait<T> {
                type Assoc<'a> where T: 'a;
            }
        };
        let nested_trait_impl = parse_quote! {
            impl<T: 'static> NestedTrait<T> for T {
                type Assoc<'a> = T::Assoc<'a> where T: 'a;
            }
        };
        file.items.insert(0, Item::Trait(nested_trait));
        file.items.insert(1, Item::Impl(nested_trait_impl));

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for pred in &mut where_clause.predicates {
                        if let syn::WherePredicate::For(for_pred) = pred {
                            if let syn::WherePredicate::Type(type_pred) = &mut *for_pred.predicate {
                                if let syn::Type::Path(type_path) = &type_pred.bounded_ty {
                                    for segment in &type_path.path.segments {
                                        if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                                            for arg in &args.args {
                                                if let syn::GenericArgument::Lifetime(_) = arg {
                                                    let bounded_ty = &type_pred.bounded_ty;
                                                    let new_trait_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                                                        path: {
                                                            let mut path = syn::Path::from(Ident::new("NestedTrait", proc_macro2::Span::call_site()));
                                                            path.segments.push(syn::PathSegment {
                                                                ident: Ident::new("NestedTrait", proc_macro2::Span::call_site()),
                                                                arguments: syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                                    colon2_token: None,
                                                                    lt_token: token::Lt::default(),
                                                                    args: {
                                                                        let mut args = Punctuated::new();
                                                                        args.push(syn::GenericArgument::Type(bounded_ty.clone()));
                                                                        args
                                                                    },
                                                                    gt_token: token::Gt::default(),
                                                                }),
                                                            });
                                                            path
                                                        },
                                                        modifier: syn::TraitBoundModifier::None,
                                                        lifetimes: None,
                                                        paren_token: None,
                                                    });
                                                    let new_for_pred = syn::WherePredicate::For(syn::For {
                                                        for_token: for_pred.for_token,
                                                        lifetimes: for_pred.lifetimes.clone(),
                                                        colon_token: for_pred.colon_token,
                                                        predicate: Box::new(syn::WherePredicate::Type(syn::PredicateType {
                                                            lifetimes: None,
                                                            bounded_ty: bounded_ty.clone(),
                                                            colon_token: Default::default(),
                                                            bounds: {
                                                                let mut bounds = Punctuated::new();
                                                                bounds.push(new_trait_bound);
                                                                bounds
                                                            },
                                                        })),
                                                    });
                                                    where_clause.predicates.push(new_for_pred);
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
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for pred in &mut where_clause.predicates {
                                if let syn::WherePredicate::For(for_pred) = pred {
                                    if let syn::WherePredicate::Type(type_pred) = &mut *for_pred.predicate {
                                        if let syn::Type::Path(type_path) = &type_pred.bounded_ty {
                                            for segment in &type_path.path.segments {
                                                if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                                                    for arg in &args.args {
                                                        if let syn::GenericArgument::Lifetime(_) = arg {
                                                            let bounded_ty = &type_pred.bounded_ty;
                                                            let new_trait_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                                                                path: {
                                                                    let mut path = syn::Path::from(Ident::new("NestedTrait", proc_macro2::Span::call_site()));
                                                                    path.segments.push(syn::PathSegment {
                                                                        ident: Ident::new("NestedTrait", proc_macro2::Span::call_site()),
                                                                        arguments: syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                                            colon2_token: None,
                                                                            lt_token: token::Lt::default(),
                                                                            args: {
                                                                                let mut args = Punctuated::new();
                                                                                args.push(syn::GenericArgument::Type(bounded_ty.clone()));
                                                                                args
                                                                            },
                                                                            gt_token: token::Gt::default(),
                                                                        }),
                                                                    });
                                                                    path
                                                                },
                                                                modifier: syn::TraitBoundModifier::None,
                                                                lifetimes: None,
                                                                paren_token: None,
                                                            });
                                                            let new_for_pred = syn::WherePredicate::For(syn::For {
                                                                for_token: for_pred.for_token,
                                                                lifetimes: for_pred.lifetimes.clone(),
                                                                colon_token: for_pred.colon_token,
                                                                predicate: Box::new(syn::WherePredicate::Type(syn::PredicateType {
                                                                    lifetimes: None,
                                                                    bounded_ty: bounded_ty.clone(),
                                                                    colon_token: Default::default(),
                                                                    bounds: {
                                                                        let mut bounds = Punctuated::new();
                                                                        bounds.push(new_trait_bound);
                                                                        bounds
                                                                    },
                                                                })),
                                                            });
                                                            where_clause.predicates.push(new_for_pred);
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
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces recursive higher-ranked trait bounds with nested GAT references by adding a new for<'a> bound to existing where clauses involving GATs. It appends a NestedTrait bound that recursively references the same GAT, creating circular dependencies that may trigger type inference errors or ICEs in region checking. The transformation leverages existing GAT definitions and introduces a minimal trait to force recursive resolution attempts."
    }
}