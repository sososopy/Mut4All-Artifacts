use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
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

pub struct Replace_Await_Expression_17;

impl Mutator for Replace_Await_Expression_17 {
    fn name(&self) -> &str {
        "Replace_Await_Expression_17"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(expr) = stmt {
                            if let Expr::Await(await_expr) = &**expr {
                                let block_on_path = SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("block_on", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                };
                                let block_on_expr = Expr::Path(ExprPath {
                                    attrs: vec![],
                                    path: block_on_path,
                                });
                                let block_on_call = Expr::Call(ExprCall {
                                    attrs: vec![],
                                    expr: Box::new(block_on_expr),
                                    paren_token: Paren {
                                        span: Span::call_site(),
                                    },
                                    args: {
                                        let mut args = Punctuated::new();
                                        args.push(GenericArgument::Expr(&await_expr.base));
                                        args
                                    },
                                });
                                *expr = block_on_call;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions and replaces await expressions with a blocking call to the awaited future using `block_on`. This transformation removes the asynchronous behavior and tests the compiler's handling of async/await functionality."
    }
}