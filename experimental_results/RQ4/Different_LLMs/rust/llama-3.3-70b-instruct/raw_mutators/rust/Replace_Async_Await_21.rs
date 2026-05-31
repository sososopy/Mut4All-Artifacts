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

pub struct Replace_Async_Await_21;

impl Mutator for Replace_Async_Await_21 {
    fn name(&self) -> &str {
        "Replace_Async_Await_21"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                // Check if the function is async
                if func.sig.asyncness.is_some() {
                    // Replace async with non-async
                    func.sig.asyncness = None;
                } else {
                    // Replace non-async with async
                    func.sig.asyncness = Some(syn::Async::default());
                }
                // Check if the function contains an await expression
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Await(await_expr) = expr {
                            // Replace await with a non-await expression
                            *expr = Expr::Path(ExprPath {
                                attrs: await_expr.attrs.clone(),
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("std", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                                qself: None,
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces async functions with non-async functions and vice versa, and replaces await expressions with non-await expressions. This transformation aims to test the compiler's handling of async/await functionality and its ability to correctly convert between asynchronous and synchronous code."
    }
}