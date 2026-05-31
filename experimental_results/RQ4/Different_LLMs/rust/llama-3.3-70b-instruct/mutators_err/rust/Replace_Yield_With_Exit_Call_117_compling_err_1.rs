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

pub struct Replace_Yield_With_Exit_Call_117;

impl Mutator for Replace_Yield_With_Exit_Call_117 {
    fn name(&self) -> &str {
        "Replace_Yield_With_Exit_Call_117"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Yield(yield_expr) = &**expr {
                            let exit_call: Expr = parse_quote! {
                                std::process::exit(0)
                            };
                            *expr = Box::new(exit_call);
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr) = stmt {
                                if let Expr::Yield(yield_expr) = &**expr {
                                    let exit_call: Expr = parse_quote! {
                                        std::process::exit(0)
                                    };
                                    *expr = Box::new(exit_call);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces yield statements with a call to std::process::exit(0) in functions and methods. This transformation introduces potential issues with borrow checking and lifetime analysis, testing the compiler's ability to handle complex scenarios involving coroutines and generators."
    }
}