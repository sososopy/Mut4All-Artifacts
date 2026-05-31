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
    token::{Lt, Gt},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Union_Array_Length_With_Generic_Const_340;

impl Mutator for Replace_Union_Array_Length_With_Generic_Const_340 {
    fn name(&self) -> &str {
        "Replace_Union_Array_Length_With_Generic_Const_340"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(union) = item {
                let has_type_params = union.generics.params.iter().any(|param| {
                    if let syn::GenericParam::Type(_) = param {
                        true
                    } else {
                        false
                    }
                });
                if !has_type_params {
                    continue;
                }

                let first_type_ident = union.generics.params.iter().find_map(|param| {
                    if let syn::GenericParam::Type(type_param) = param {
                        Some(&type_param.ident)
                    } else {
                        None
                    }
                }).unwrap();

                let mut fields_to_process = Vec::new();
                if let syn::Fields::Named(named) = Box::new(&mut union.fields).as_mut() {
                    for field in &mut named.named {
                        if let Type::Array(array) = &mut field.ty {
                            fields_to_process.push(field);
                        }
                    }
                } else if let syn::Fields::Unnamed(unnamed) = Box::new(&mut union.fields).as_mut() {
                    for field in &mut unnamed.unnamed {
                        if let Type::Array(array) = &mut field.ty {
                            fields_to_process.push(field);
                        }
                    }
                }

                for field in fields_to_process {
                    if let Type::Array(array) = &mut field.ty {
                        let path = syn::Path {
                            leading_colon: None,
                            segments: {
                                let mut segments = Punctuated::new();
                                segments.push(PathSegment {
                                    ident: Ident::new("mem", Span::call_site()),
                                    arguments: PathArguments::None,
                                });
                                segments.push(PathSegment {
                                    ident: Ident::new("size_of", Span::call_site()),
                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: Lt::default(),
                                        args: Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: first_type_ident.clone(),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        }))]),
                                        gt_token: Gt::default(),
                                    }),
                                });
                                segments
                            },
                        };

                        let expr_call = syn::Expr::Call(ExprCall {
                            attrs: Vec::new(),
                            func: Box::new(syn::Expr::Path(ExprPath {
                                attrs: Vec::new(),
                                qself: None,
                                path,
                            })),
                            paren_token: Default::default(),
                            args: Punctuated::new(),
                        });

                        let expr_block = syn::Expr::Block(ExprBlock {
                            attrs: Vec::new(),
                            label: None,
                            block: parse_quote!({ #expr_call }),
                        });

                        let expr_const = syn::Expr::Const(ExprConst {
                            const_token: Default::default(),
                            value: Box::new(expr_block),
                        });

                        array.len = expr_const;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}