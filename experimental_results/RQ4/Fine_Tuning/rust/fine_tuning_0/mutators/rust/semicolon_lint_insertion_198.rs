use proc_macro2::{Span, TokenStream, TokenTree};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default::Default, fs, ops::Range, panic, path::Path, process::Command};
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
    Attribute,
};

use crate::mutator::Mutator;

pub struct Semicolon_Lint_Insertion_198;

impl Mutator for Semicolon_Lint_Insertion_198 {
    fn name(&self) -> &str {
        "Semicolon_Lint_Insertion_198"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                if let Some(token) = macro_item.mac.tokens.clone().into_iter().next() {
                    if let TokenTree::Group(group) = token {
                        let mut new_stream = TokenStream::new();
                        for token in group.stream() {
                            new_stream.extend(Some(token.clone()));
                            if let TokenTree::Punct(punct) = &token {
                                if punct.as_char() == ';' {
                                    new_stream.extend(Some(TokenTree::Punct(punct.clone())));
                                }
                            }
                        }
                        macro_item.mac.tokens = new_stream;
                    }
                }
            }
            if let Item::Fn(func) = item {
                let mut new_block = func.block.clone();
                let mut last_stmt_index = None;
                for (i, stmt) in new_block.stmts.iter_mut().enumerate() {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Macro(expr_macro) = expr {
                            let macro_name = expr_macro.mac.path.segments.last().unwrap().ident.to_string();
                            if macro_name.contains("calculate") {
                                let warn_attr: Attribute = parse_quote!(#[warn(semicolon_in_expressions_from_macros)]);
                                func.attrs.push(warn_attr);
                                last_stmt_index = Some(i);
                            }
                        }
                    }
                }
                if let Some(i) = last_stmt_index {
                    if let Stmt::Expr(expr, _) = &mut new_block.stmts[i] {
                        new_block.stmts[i] = Stmt::Expr(expr.clone(), Some(Default::default()));
                    }
                }
                func.block = new_block;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macros and their usage within async blocks or functions. It seeks out macro invocations and applies a semicolon lint warning attribute. Additionally, it ensures that a semicolon is appended to macro invocations if not already present, temporarily removing any existing semicolon-related attributes to highlight the mutation's effect."
    }
}