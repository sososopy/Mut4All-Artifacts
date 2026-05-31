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

pub struct Replace_Memcpy_With_Memmove_3;

impl Mutator for Replace_Memcpy_With_Memmove_3 {
    fn name(&self) -> &str {
        "Replace_Memcpy_With_Memmove_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call) = expr {
                            if let Expr::Path(path) = &call.func {
                                if path.path.segments.last().unwrap().ident == "memcpy" {
                                    let new_path = SynPath {
                                        leading_colon: path.path.leading_colon,
                                        segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("memmove", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    };
                                    let new_expr = Expr::Path(ExprPath {
                                        attrs: path.attrs.clone(),
                                        qself: None,
                                        path: new_path,
                                    });
                                    let new_call = Expr::Call(ExprCall {
                                        attrs: call.attrs.clone(),
                                        func: Box::new(new_expr),
                                        paren_token: call.paren_token,
                                        args: call.args.clone(),
                                    });
                                    *expr = Box::new(new_call);
                                } else if path.path.segments.last().unwrap().ident == "memmove" {
                                    let new_path = SynPath {
                                        leading_colon: path.path.leading_colon,
                                        segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("memcpy", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    };
                                    let new_expr = Expr::Path(ExprPath {
                                        attrs: path.attrs.clone(),
                                        qself: None,
                                        path: new_path,
                                    });
                                    let new_call = Expr::Call(ExprCall {
                                        attrs: call.attrs.clone(),
                                        func: Box::new(new_expr),
                                        paren_token: call.paren_token,
                                        args: call.args.clone(),
                                    });
                                    *expr = Box::new(new_call);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces instances of `memcpy` with `memmove` and vice versa in the given code. This transformation tests the compiler's ability to handle different memory operations and can help expose bugs related to memory management and optimization."
    }
}