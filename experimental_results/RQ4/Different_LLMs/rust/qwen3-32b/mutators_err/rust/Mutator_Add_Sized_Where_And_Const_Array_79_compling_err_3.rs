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

pub struct Mutator_Add_Sized_Where_And_Const_Array_79;

impl Mutator for Mutator_Add_Sized_Where_And_Const_Array_79 {
    fn name(&self) -> &str {
        "Mutator_Add_Sized_Where_And_Const_Array_79"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                if generics.params.is_empty() {
                    continue;
                }
                if let Some(first_param) = generics.params.first() {
                    if let syn::GenericParam::Type(type_param) = first_param {
                        let t_type = syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = syn::punctuated::Punctuated::new();
                                    segs.push(syn::PathSegment {
                                        ident: type_param.ident.clone(),
                                        arguments: syn::PathArguments::None,
                                    });
                                    segs
                                },
                            },
                        });
                        let sized_bound = syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = syn::punctuated::Punctuated::new();
                                    segs.push(syn::PathSegment {
                                        ident: syn::Ident::new("Sized", Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    segs
                                },
                            },
                        };
                        let mut where_clause = generics.where_clause.take().unwrap_or_else(|| {
                            syn::WhereClause {
                                where_token: Default::default(),
                                predicates: syn::punctuated::Punctuated::new(),
                            }
                        });
                        let predicate = syn::WherePredicate::Type(syn::PredicateType {
                            lifetimes: None,
                            bounded_ty: t_type.clone(),
                            colon_token: Default::default(),
                            bounds: {
                                let mut bounds = syn::punctuated::Punctuated::new();
                                bounds.push(syn::TypeParamBound::Trait(sized_bound));
                                bounds
                            },
                        });
                        where_clause.predicates.push(predicate);
                        generics.where_clause = Some(where_clause);
                        let option_t_type = syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = syn::punctuated::Punctuated::new();
                                    segs.push(syn::PathSegment {
                                        ident: syn::Ident::new("Option", Span::call_site()),
                                        arguments: syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: syn::token::Lt::default(),
                                            args: {
                                                let mut args = syn::punctuated::Punctuated::new();
                                                args.push(syn::GenericArgument::Type(t_type));
                                                args
                                            },
                                            gt_token: syn::token::Gt::default(),
                                        }),
                                    });
                                    segs
                                },
                            },
                        });
                        let array_expr = parse_quote! {
                            [(); { let _x: #option_t_type = None; 0 }]
                        };
                        if let syn::Block { stmts, .. } = &mut func.block {
                            stmts.push(syn::Stmt::Expr(array_expr, None));
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