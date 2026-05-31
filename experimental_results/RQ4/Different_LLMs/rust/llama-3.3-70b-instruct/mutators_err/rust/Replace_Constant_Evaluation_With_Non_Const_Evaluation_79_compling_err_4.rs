use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes,
    Expr,
    ExprCall,
    ExprPath,
    File,
    FnArg,
    GenericArgument,
    GenericParam,
    Ident,
    Item,
    ItemFn,
    ItemStruct,
    Lifetime,
    LifetimeParam,
    Local,
    Pat,
    PatType,
    Path as SynPath,
    PathArguments,
    ReturnType,
    Stmt,
    TraitBound,
    TraitBoundModifier,
    Type,
    TypeImplTrait,
    TypeParamBound,
    TypePath,
    parse_quote,
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

pub struct Replace_Constant_Evaluation_With_Non_Const_Evaluation_79;

impl Mutator for Replace_Constant_Evaluation_With_Non_Const_Evaluation_79 {
    fn name(&self) -> &str {
        "Replace_Constant_Evaluation_With_Non_Const_Evaluation_79"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                let mut new_item = syn::Item::Let(syn::Let {
                    attrs: const_item.attrs.clone(),
                    let_token: token::Let {
                        span: const_item.ident.span(),
                    },
                    pat: Box::new(Pat::Ident(PatIdent {
                        attrs: vec![],
                        by_ref: None,
                        mutability: None,
                        subpat: None,
                        ident: const_item.ident.clone(),
                    })),
                    eq_token: token::Eq {
                        spans: [const_item.ident.span()],
                    },
                    expr: const_item.expr.clone(),
                    semi_token: token::Semi {
                        spans: [const_item.ident.span()],
                    },
                });
                *item = syn::Item::Let(new_item);
            }
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(ExprCall {
                            attrs: _,
                            func: expr_func,
                            paren_token: _,
                            args: _,
                        }) = expr {
                            if let Expr::Path(ExprPath {
                                attrs: _,
                                qself: _,
                                path: SynPath {
                                    leading_colon: _,
                                    segments: segments,
                                },
                            }) = **expr_func {
                                if let Some(segment) = segments.iter().next() {
                                    if segment.ident.to_string().starts_with("const_") {
                                        let new_expr = parse_quote! {
                                            {
                                                let x = 0;
                                                x
                                            }
                                        };
                                        *expr = new_expr;
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
        "The mutation operator replaces constant expressions with non-const expressions. It targets constant functions, variables, and literals, replacing them with non-const equivalents. This transformation aims to test the compiler's ability to handle constant evaluation and non-const evaluation correctly, potentially exposing bugs related to constant folding, optimization, and error reporting."
    }
}