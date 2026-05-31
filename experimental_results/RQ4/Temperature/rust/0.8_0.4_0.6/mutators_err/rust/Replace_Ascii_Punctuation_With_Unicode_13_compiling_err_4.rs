use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Replace_Ascii_Punctuation_With_Unicode_13;

impl Mutator for Replace_Ascii_Punctuation_With_Unicode_13 {
    fn name(&self) -> &str {
        "Replace_Ascii_Punctuation_With_Unicode_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    replace_ascii_with_unicode(&mut item_fn.block.stmts);
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces ASCII punctuation characters with their Unicode equivalents in function calls, array indexing, and block delimiters. This transformation aims to test the compiler's handling of Unicode characters that resemble standard ASCII punctuation, potentially leading to parsing errors or internal compiler errors."
    }
}

fn replace_ascii_with_unicode(stmts: &mut Vec<Stmt>) {
    for stmt in stmts {
        match stmt {
            Stmt::Expr(expr, _) | Stmt::Semi(expr, _) => {
                replace_in_expr(expr);
            }
            Stmt::Local(local) => {
                if let Some(LocalInit { expr, .. }) = &mut local.init {
                    replace_in_expr(expr);
                }
            }
            _ => {}
        }
    }
}

fn replace_in_expr(expr: &mut Expr) {
    match expr {
        Expr::Call(expr_call) => {
            replace_in_call(expr_call);
        }
        Expr::Index(expr_index) => {
            replace_in_expr(&mut *expr_index.expr);
            replace_in_expr(&mut *expr_index.index);
        }
        Expr::Block(expr_block) => {
            replace_ascii_with_unicode(&mut expr_block.block.stmts);
        }
        _ => {}
    }
}

fn replace_in_call(expr_call: &mut ExprCall) {
    let mut new_args = Punctuated::new();
    for arg in expr_call.args.iter_mut() {
        replace_in_expr(arg);
        new_args.push_value(arg.clone());
        new_args.push_punct(Comma(Span::call_site()));
    }
    expr_call.args = new_args;
}

fn replace_ascii_with_unicode_in_ident(ident: &mut Ident) {
    let new_ident = ident.to_string().replace("(", "（").replace(")", "）").replace("[", "［").replace("]", "］").replace("{", "｛").replace("}", "｝");
    *ident = Ident::new(&new_ident, ident.span());
}