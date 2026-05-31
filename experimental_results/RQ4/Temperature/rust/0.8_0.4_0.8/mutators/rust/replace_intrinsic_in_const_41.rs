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

pub struct Replace_Intrinsic_In_Const_41;

impl Mutator for Replace_Intrinsic_In_Const_41 {
    fn name(&self) -> &str {
        "Replace_Intrinsic_In_Const_41"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(item_const) = item {
                if let syn::Expr::Block(expr_block) = &*item_const.expr {
                    let mut new_stmts = vec![];
                    for stmt in &expr_block.block.stmts {
                        if let syn::Stmt::Expr(syn::Expr::Call(expr_call), _) = stmt {
                            if let syn::Expr::Path(expr_path) = &*expr_call.func {
                                if expr_path.path.segments.len() == 3
                                    && expr_path.path.segments[0].ident == "core"
                                    && expr_path.path.segments[1].ident == "mem"
                                    && expr_path.path.segments[2].ident == "transmute"
                                {
                                    let new_func: Expr = parse_quote! { my_transmute };
                                    let new_call = ExprCall {
                                        attrs: expr_call.attrs.clone(),
                                        func: Box::new(new_func),
                                        paren_token: expr_call.paren_token,
                                        args: expr_call.args.clone(),
                                    };
                                    new_stmts.push(Stmt::Expr(Expr::Call(new_call), None));
                                    continue;
                                }
                            }
                        }
                        new_stmts.push(stmt.clone());
                    }
                    item_const.expr = Box::new(Expr::Block(syn::ExprBlock {
                        attrs: expr_block.attrs.clone(),
                        label: expr_block.label.clone(),
                        block: syn::Block {
                            brace_token: expr_block.block.brace_token,
                            stmts: new_stmts,
                        },
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline `const` blocks containing intrinsic function calls, specifically `core::mem::transmute`. It replaces these with calls to a user-defined equivalent function `my_transmute`. This transformation tests the compiler's handling of intrinsic functions within `const` contexts and ensures that equivalent non-intrinsic functions can be substituted without altering program behavior."
    }
}