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

pub struct AnonConstNameConflictInTrait_280;

impl Mutator for AnonConstNameConflictInTrait_280 {
    fn name(&self) -> &str {
        "AnonConstNameConflictInTrait_280"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let type_params: Vec<_> = trait_item
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let GenericParam::Type(type_param) = param {
                            Some(type_param.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if type_params.is_empty() {
                    continue;
                }
                let mut rng = thread_rng();
                let target_name = type_params
                    .choose(&mut rng)
                    .expect("No type parameters found in trait");
                for param in &mut trait_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let new_type = Type::Path(TypePath {
                            qself: None,
                            path: Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = Punctuated::new();
                                    segs.push(PathSegment {
                                        ident: target_name.clone(),
                                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: {
                                                let mut args = Punctuated::new();
                                                args.push(GenericArgument::Type(Type::Path(TypePath {
                                                    qself: None,
                                                    path: Path {
                                                        leading_colon: None,
                                                        segments: {
                                                            let mut segs = Punctuated::new();
                                                            segs.push(PathSegment {
                                                                ident: Ident::new("i32", Span::call_site()),
                                                                arguments: PathArguments::None,
                                                            });
                                                            segs
                                                        },
                                                    },
                                                })));
                                                args
                                            },
                                            gt_token: token::Gt::default(),
                                        }),
                                    });
                                    segs
                                },
                            },
                        });
                        const_param.ty = Box::new(new_type);
                    }
                }
                for item in &mut trait_item.items {
                    if let TraitItem::Method(method) = item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                let new_type = Type::Path(TypePath {
                                    qself: None,
                                    path: Path {
                                        leading_colon: None,
                                        segments: {
                                            let mut segs = Punctuated::new();
                                            segs.push(PathSegment {
                                                ident: target_name.clone(),
                                                arguments: PathArguments::None,
                                            });
                                            segs
                                        },
                                    },
                                });
                                pat_type.ty = Box::new(new_type);
                            }
                        }
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            let new_type = Type::Path(TypePath {
                                qself: None,
                                path: Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(PathSegment {
                                            ident: target_name.clone(),
                                            arguments: PathArguments::None,
                                        });
                                        segs
                                    },
                                },
                            });
                            *return_type = Box::new(new_type);
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