use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *
};
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

pub struct Replace_Const_Value_466;

impl Mutator for Replace_Const_Value_466 {
    fn name(&self) -> &str {
        "Replace_Const_Value_466"
    }

    fn chain_of_thought(&self) -> &str {
        "Replace const values in function arguments"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }) = &*pat_type.ty
                        {
                            if let Some(PathSegment {
                                ident,
                                arguments,
                            }) = segments.first()
                            {
                                if ident.to_string() == "is_transmutable" {
                                    if let PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        args,
                                        ..
                                    }) = arguments
                                    {
                                        for arg in args {
                                            if let GenericArgument::Const(expr) = arg
                                            {
                                                let mut rng = thread_rng();
                                                let new_value = match expr {
                                                    syn::Expr::Lit(syn::ExprLit {
                                                        lit: syn::Lit::Int(int),
                                                        attrs,
                                                    }) => {
                                                        let new_int = int.base10_parse::<i32>().unwrap();
                                                        let new_value = if new_int == 0 {
                                                            1
                                                        } else {
                                                            0
                                                        };
                                                        syn::Expr::Lit(syn::ExprLit {
                                                            lit: syn::Lit::Int(syn::LitInt::new(
                                                                &new_value.to_string(),
                                                                Span::call_site(),
                                                            )),
                                                            attrs,
                                                        })
                                                    }
                                                    _ => panic!("Unsupported expression type"),
                                                };
                                                *expr = new_value;
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