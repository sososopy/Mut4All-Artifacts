use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token::{Paren, Plus, Comma, Lt, Gt},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

pub struct Add_Nested_AssocType_WhereClauses_137;

impl Mutator for Add_Nested_AssocType_WhereClauses_137 {
    fn name(&self) -> &str {
        "Add_Nested_AssocType_WhereClauses_137"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    // Check if there's at least one PredicateType in the where clause
                    if let Some(pred) = where_clause.predicates.iter().find_map(|p| {
                        if let syn::WherePredicate::Type(_) = p {
                            Some(p)
                        } else {
                            None
                        }
                    }) {
                        // Add new type parameter V
                        let v_ident = Ident::new("V", Span::call_site());
                        func.sig.generics.params.push(GenericParam::Type(syn::TypeParam {
                            attrs: Vec::new(),
                            ident: v_ident.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                            eq_token: None,
                            default: None,
                        }));

                        // Create new PredicateType for nested where clause
                        let new_pred = syn::WherePredicate::Type(syn::PredicateType {
                            lifetimes: None,
                            bounded_ty: Box::new(syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(syn::PathSegment {
                                            ident: Ident::new("T", Span::call_site()),
                                            arguments: PathArguments::None,
                                        });
                                        segs.push(syn::PathSegment {
                                            ident: Ident::new("Assoc", Span::call_site()),
                                            arguments: PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: Lt::default(),
                                                args: {
                                                    let mut args = Punctuated::<GenericArgument, Comma>::new();
                                                    args.push(GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                                        qself: None,
                                                        path: syn::Path {
                                                            leading_colon: None,
                                                            segments: {
                                                                let mut segs2 = Punctuated::new();
                                                                segs2.push(syn::PathSegment {
                                                                    ident: Ident::new("U", Span::call_site()),
                                                                    arguments: PathArguments::None,
                                                                });
                                                                segs2
                                                            },
                                                        },
                                                    }));
                                                    args
                                                },
                                                gt_token: Gt::default(),
                                            }),
                                        });
                                        segs
                                    },
                                },
                            })),
                            colon_token: token::Colon::default(),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                let trait_bound = syn::TraitBound {
                                    paren_token: None,
                                    modifier: syn::TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: {
                                            let mut segs = Punctuated::new();
                                            segs.push(syn::PathSegment {
                                                ident: Ident::new("Example", Span::call_site()),
                                                arguments: PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: Lt::default(),
                                                    args: {
                                                        let mut args2 = Punctuated::<GenericArgument, Comma>::new();
                                                        args2.push(GenericArgument::AssocType(syn::AssocType {
                                                            eq_token: token::Eq::default(),
                                                            ident: Ident::new("Assoc", Span::call_site()),
                                                            gen_args: PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                                colon2_token: None,
                                                                lt_token: Lt::default(),
                                                                args: {
                                                                    let mut args3 = Punctuated::<GenericArgument, Comma>::new();
                                                                    args3.push(GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                                                        qself: None,
                                                                        path: syn::Path {
                                                                            leading_colon: None,
                                                                            segments: {
                                                                                let mut segs3 = Punctuated::new();
                                                                                segs3.push(syn::PathSegment {
                                                                                    ident: Ident::new("V", Span::call_site()),
                                                                                    arguments: PathArguments::None,
                                                                                });
                                                                                segs3
                                                                            },
                                                                        },
                                                                    }));
                                                                    args3
                                                                },
                                                                gt_token: Gt::default(),
                                                            }),
                                                            colon_token: None,
                                                        }));
                                                        args2
                                                    },
                                                    gt_token: Gt::default(),
                                                },
                                            });
                                            segs
                                        },
                                    },
                                };
                                bounds.push(syn::TypeParamBound::Trait(trait_bound));
                                bounds
                            },
                        });

                        // Add the new predicate to the where clause
                        where_clause.predicates.push(new_pred);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}