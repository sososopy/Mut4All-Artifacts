use proc_macro2::{Span, proc_macro2::extra::DelimSpan};
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

pub struct Replace_Const_With_Computation_70;

impl Mutator for Replace_Const_With_Computation_70 {
    fn name(&self) -> &str {
        "Replace_Const_With_Computation_70"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let Some(expr) = &const_item.expr {
                    if let Expr::Lit(lit) = expr.as_ref() {
                        if let Lit::Int(int) = &lit.lit {
                            let func_name = Ident::new("some_function_returning_i32", Span::call_site());
                            let func_call = Expr::Call(ExprCall {
                                attrs: vec![],
                                func: Box::new(Expr::Path(ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: func_name,
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                })),
                                paren_token: Paren {
                                    span: DelimSpan::call_site(),
                                },
                                args: punctuated::Punctuated::new(),
                            });
                            const_item.expr = Box::new(func_call);
                        }
                    }
                }
            }
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Lit(lit) = expr {
                            if let Lit::Int(int) = &lit.lit {
                                let func_name = Ident::new("some_function_returning_i32", Span::call_site());
                                let func_call = Expr::Call(ExprCall {
                                    attrs: vec![],
                                    func: Box::new(Expr::Path(ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: func_name,
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    })),
                                    paren_token: Paren {
                                        span: DelimSpan::call_site(),
                                    },
                                    args: punctuated::Punctuated::new(),
                                });
                                *expr = func_call;
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Lit(lit) = expr {
                                    if let Lit::Int(int) = &lit.lit {
                                        let func_name = Ident::new("some_function_returning_i32", Span::call_site());
                                        let func_call = Expr::Call(ExprCall {
                                            attrs: vec![],
                                            func: Box::new(Expr::Path(ExprPath {
                                                attrs: vec![],
                                                qself: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: func_name,
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            })),
                                            paren_token: Paren {
                                                span: DelimSpan::call_site(),
                                            },
                                            args: punctuated::Punctuated::new(),
                                        });
                                        *expr = func_call;
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
        "The mutation operator replaces constant expressions with function calls, aiming to stress the constant evaluation system of the Rust compiler."
    }
}