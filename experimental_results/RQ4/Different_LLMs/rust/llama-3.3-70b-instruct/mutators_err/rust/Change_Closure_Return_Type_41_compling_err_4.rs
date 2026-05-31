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
    token::{Paren, RArrow},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Change_Closure_Return_Type_41;

impl Mutator for Change_Closure_Return_Type_41 {
    fn name(&self) -> &str {
        "Change_Closure_Return_Type_41"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for statement in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = statement {
                        if let Expr::Closure(closure) = expr {
                            let new_return_type = match closure.output {
                                ReturnType::Default => parse_quote!(String),
                                ReturnType::Type(_, ty) => {
                                    if let Type::Path(ref path) = *ty {
                                        if path.path.segments.last().unwrap().ident == "i32" {
                                            parse_quote!(String)
                                        } else if path.path.segments.last().unwrap().ident == "String" {
                                            parse_quote!(i32)
                                        } else {
                                            parse_quote!(bool)
                                        }
                                    } else {
                                        parse_quote!(String)
                                    }
                                }
                            };
                            closure.output = ReturnType::Type(RArrow::default(), new_return_type);
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for statement in &mut method.block.stmts {
                            if let Stmt::Expr(expr, _) = statement {
                                if let Expr::Closure(closure) = expr {
                                    let new_return_type = match closure.output {
                                        ReturnType::Default => parse_quote!(String),
                                        ReturnType::Type(_, ty) => {
                                            if let Type::Path(ref path) = *ty {
                                                if path.path.segments.last().unwrap().ident == "i32" {
                                                    parse_quote!(String)
                                                } else if path.path.segments.last().unwrap().ident == "String" {
                                                    parse_quote!(i32)
                                                } else {
                                                    parse_quote!(bool)
                                                }
                                            } else {
                                                parse_quote!(String)
                                            }
                                        }
                                    };
                                    closure.output = ReturnType::Type(RArrow::default(), new_return_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the return type of closures to a type that is not compatible with the original return type. This transformation aims to test the compiler's handling of closure return types and type checking."
    }
}