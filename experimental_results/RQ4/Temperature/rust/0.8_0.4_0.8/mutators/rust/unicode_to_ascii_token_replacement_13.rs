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

pub struct Unicode_To_ASCII_Token_Replacement_13;

impl Mutator for Unicode_To_ASCII_Token_Replacement_13 {
    fn name(&self) -> &str {
        "Unicode_To_ASCII_Token_Replacement_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = UnicodeToASCIITokenVisitor;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator systematically replaces Unicode characters that resemble ASCII characters with their ASCII equivalents, focusing on function calls and parentheses usage. It aims to uncover bugs related to Unicode handling by ensuring that visually similar Unicode characters are correctly interpreted as their intended ASCII counterparts."
    }
}

struct UnicodeToASCIITokenVisitor;

impl VisitMut for UnicodeToASCIITokenVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        let mut new_args = Punctuated::new();
        for arg in node.args.iter_mut() {
            self.visit_expr_mut(arg);
            new_args.push(arg.clone());
        }
        node.args = new_args;
        visit_mut::visit_expr_call_mut(self, node);
    }

    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Paren(expr_paren) = node {
            let span = expr_paren.paren_token.span;
            expr_paren.paren_token = Paren { span };
        }
        visit_mut::visit_expr_mut(self, node);
    }
}