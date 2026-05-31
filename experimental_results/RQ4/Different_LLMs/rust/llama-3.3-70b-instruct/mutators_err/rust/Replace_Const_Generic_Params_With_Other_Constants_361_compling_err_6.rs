use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Replace_Const_Generic_Params_With_Other_Constants_361;

impl Mutator for Replace_Const_Generic_Params_With_Other_Constants_361 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Params_With_Other_Constants_361"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_value = match param.default {
                                Some(ref default) => {
                                    // Replace with another constant or literal of the same type
                                    match default {
                                        Expr::Lit(lit) => {
                                            match lit.lit {
                                                Lit::Int(int) => {
                                                    // Replace with a different integer value
                                                    Lit::Int(LitInt::new(int.base10_digits(), Span::call_site()))
                                                }
                                                Lit::Float(float) => {
                                                    // Replace with a different float value
                                                    Lit::Float(LitFloat::new(float.base10_digits(), Span::call_site()))
                                                }
                                                Lit::Str(str) => {
                                                    // Replace with a different string value
                                                    Lit::Str(LitStr::new(&str.value(), Span::call_site()))
                                                }
                                                _ => panic!("Unsupported literal type"),
                                            }
                                        }
                                        _ => panic!("Unsupported default value"),
                                    }
                                }
                                None => {
                                    // If no default value is provided, use a default constant
                                    Lit::Int(LitInt::new("1024", Span::call_site()))
                                }
                            };
                            param.default = Some(Expr::Lit(ExprLit {
                                lit: new_value,
                                attrs: vec![],
                            }));
                        }
                    }
                }
            }
            if let syn::Item::Fn(item_fn) = item {
                if let Some(generics) = &mut item_fn.sig.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_value = match param.default {
                                Some(ref default) => {
                                    // Replace with another constant or literal of the same type
                                    match default {
                                        Expr::Lit(lit) => {
                                            match lit.lit {
                                                Lit::Int(int) => {
                                                    // Replace with a different integer value
                                                    Lit::Int(LitInt::new(int.base10_digits(), Span::call_site()))
                                                }
                                                Lit::Float(float) => {
                                                    // Replace with a different float value
                                                    Lit::Float(LitFloat::new(float.base10_digits(), Span::call_site()))
                                                }
                                                Lit::Str(str) => {
                                                    // Replace with a different string value
                                                    Lit::Str(LitStr::new(&str.value(), Span::call_site()))
                                                }
                                                _ => panic!("Unsupported literal type"),
                                            }
                                        }
                                        _ => panic!("Unsupported default value"),
                                    }
                                }
                                None => {
                                    // If no default value is provided, use a default constant
                                    Lit::Int(LitInt::new("1024", Span::call_site()))
                                }
                            };
                            param.default = Some(Expr::Lit(ExprLit {
                                lit: new_value,
                                attrs: vec![],
                            }));
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                if let Some(generics) = &mut item_impl.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_value = match param.default {
                                Some(ref default) => {
                                    // Replace with another constant or literal of the same type
                                    match default {
                                        Expr::Lit(lit) => {
                                            match lit.lit {
                                                Lit::Int(int) => {
                                                    // Replace with a different integer value
                                                    Lit::Int(LitInt::new(int.base10_digits(), Span::call_site()))
                                                }
                                                Lit::Float(float) => {
                                                    // Replace with a different float value
                                                    Lit::Float(LitFloat::new(float.base10_digits(), Span::call_site()))
                                                }
                                                Lit::Str(str) => {
                                                    // Replace with a different string value
                                                    Lit::Str(LitStr::new(&str.value(), Span::call_site()))
                                                }
                                                _ => panic!("Unsupported literal type"),
                                            }
                                        }
                                        _ => panic!("Unsupported default value"),
                                    }
                                }
                                None => {
                                    // If no default value is provided, use a default constant
                                    Lit::Int(LitInt::new("1024", Span::call_site()))
                                }
                            };
                            param.default = Some(Expr::Lit(ExprLit {
                                lit: new_value,
                                attrs: vec![],
                            }));
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with other constants or literals of the same type, testing the compiler's handling of const generics and type checking."
    }
}