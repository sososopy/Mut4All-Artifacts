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

pub struct Introduce_Borrow_Of_Non_Static_Value_In_Async_Context_35;

impl Mutator for Introduce_Borrow_Of_Non_Static_Value_In_Async_Context_35 {
    fn name(&self) -> &str {
        "Introduce_Borrow_Of_Non_Static_Value_In_Async_Context_35"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if func.sig.asyncness.is_some() {
                    let non_static_value = parse_quote! {
                        let non_static_value = 0;
                    };
                    let borrow_expr = parse_quote! {
                        let _borrow = &non_static_value;
                    };
                    func.block.stmts.insert(0, Stmt::Expr(non_static_value, None));
                    func.block.stmts.insert(1, Stmt::Expr(borrow_expr, None));
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        if func.sig.asyncness.is_some() {
                            let non_static_value = parse_quote! {
                                let non_static_value = 0;
                            };
                            let borrow_expr = parse_quote! {
                                let _borrow = &non_static_value;
                            };
                            func.block.stmts.insert(0, Stmt::Expr(non_static_value, None));
                            func.block.stmts.insert(1, Stmt::Expr(borrow_expr, None));
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a borrow of a non-static value in async functions or closures. It aims to test the borrow checker's handling of non-static values in async contexts, potentially triggering ICEs or inference failures in the borrow checking system."
    }
}