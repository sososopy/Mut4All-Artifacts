use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Introduce_Undefined_Const_In_Async_Assembly_174;

impl Mutator for Introduce_Undefined_Const_In_Async_Assembly_174 {
    fn name(&self) -> &str {
        "Introduce_Undefined_Const_In_Async_Assembly_174"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.asyncness.is_some() {
                    continue;
                }
                let has_lifetime = item_fn.sig.generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_)));
                if !has_lifetime {
                    continue;
                }
                let mut visitor = AsmVisitor::new();
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions with lifetime parameters. It locates inline assembly expressions (asm! macro) within these functions and modifies them by introducing an undefined const placeholder referencing an identifier not present in the scope. The mutation adjusts the asm! template string to include the new placeholder, either by adding a new placeholder or replacing an existing const placeholder with an undefined identifier. This transformation aims to replicate conditions that trigger ICEs by combining async functions with lifetime parameters and inline assembly referencing missing constants."
    }
}

struct AsmVisitor {
    undefined_const_ident: Ident,
}

impl AsmVisitor {
    fn new() -> Self {
        AsmVisitor {
            undefined_const_ident: Ident::new("UNDEF_CONST", Span::call_site()),
        }
    }
}

impl VisitMut for AsmVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Macro(expr_macro) = expr {
            if expr_macro.mac.path.is_ident("asm") {
                let mut new_tokens = Vec::new();
                let mut found_template = false;
                let tokens = expr_macro.mac.tokens.clone();
                let mut token_iter = tokens.into_iter();
                while let Some(token) = token_iter.next() {
                    if let proc_macro2::TokenTree::Literal(ref lit) = token {
                        if lit.to_string().contains("\"") {
                            found_template = true;
                            let new_template = format!("/* {{0}} */ {}", lit.to_string());
                            new_tokens.push(proc_macro2::TokenTree::Literal(proc_macro2::Literal::string(&new_template)));
                        } else {
                            new_tokens.push(token);
                        }
                    } else {
                        new_tokens.push(token);
                    }
                }
                if found_template {
                    new_tokens.push(proc_macro2::TokenTree::Ident(proc_macro2::Ident::new("const", Span::call_site())));
                    new_tokens.push(proc_macro2::TokenTree::Ident(self.undefined_const_ident.clone()));
                    expr_macro.mac.tokens = proc_macro2::TokenStream::from_iter(new_tokens);
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}