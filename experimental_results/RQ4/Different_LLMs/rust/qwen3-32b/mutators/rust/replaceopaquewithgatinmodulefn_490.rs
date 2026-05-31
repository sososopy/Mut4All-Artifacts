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

pub struct ReplaceOpaqueWithGatInModuleFn_490;

impl Mutator for ReplaceOpaqueWithGatInModuleFn_490 {
    fn name(&self) -> &str {
        "ReplaceOpaqueWithGatInModuleFn_490"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if !matches!(&**return_type, Type::ImplTrait(_)) {
                        let has_trait_bound = func.sig.generics.params.iter().any(|param| {
                            if let GenericParam::Type(type_param) = param {
                                type_param.bounds.iter().any(|bound| {
                                    matches!(bound, TypeParamBound::Trait(_))
                                })
                            } else {
                                false
                            }
                        });

                        if has_trait_bound {
                            let param_name = func.sig.generics.params.iter()
                                .find_map(|param| {
                                    if let GenericParam::Type(type_param) = param {
                                        if type_param.bounds.iter().any(|bound| {
                                            matches!(bound, TypeParamBound::Trait(_))
                                        }) {
                                            Some(type_param.ident.clone())
                                        } else {
                                            None
                                        }
                                    } else {
                                        None
                                    }
                                });

                            if let Some(param_name) = param_name {
                                *return_type = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                                    impl_token: token::Impl {
                                        span: Span::call_site(),
                                    },
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                            paren_token: None,
                                            modifier: syn::TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: syn::Path::from(Ident::new("Sized", Span::call_site())),
                                        }));
                                        bounds
                                    },
                                }));

                                let mut new_block = func.block.clone();
                                let assoc_path = syn::Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(PathSegment {
                                            ident: param_name.clone(),
                                            arguments: PathArguments::None,
                                        });
                                        segs.push(PathSegment {
                                            ident: Ident::new("Assoc", Span::call_site()),
                                            arguments: PathArguments::None,
                                        });
                                        segs
                                    },
                                };
                                new_block.stmts.push(Stmt::Local(Local {
                                    attrs: vec![],
                                    let_token: token::Let::default(),
                                    pat: Pat::Wild(PatWild {
                                        attrs: vec![],
                                        underscore_token: token::Underscore { spans: [Span::call_site(); 1] },
                                    }),
                                    init: None,
                                    semi_token: token::Semi::default(),
                                }));
                                new_block.stmts.push(Stmt::Expr(
                                    parse_quote! { std::marker::PhantomData },
                                    None,
                                ));
                                *func.block = *new_block;
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