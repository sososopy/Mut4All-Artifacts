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

pub struct Semicolon_Lint_Insertion_198;

impl Mutator for Semicolon_Lint_Insertion_198 {
    fn name(&self) -> &str {
        "Semicolon_Lint_Insertion_198"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut macro_names = HashSet::new();
        for item in &file.items {
            if let Item::Macro(macro_item) = item {
                if let Some(ident) = &macro_item.ident {
                    macro_names.insert(ident.to_string());
                }
            } else if let Item::Macro2(macro2_item) = item {
                macro_names.insert(macro2_item.ident.to_string());
            }
        }
        let mut macro_definitions = HashSet::new();
        for item in &file.items {
            if let Item::Macro(item_macro) = item {
                if let Some(ident) = &item_macro.ident {
                    macro_definitions.insert(ident.to_string());
                }
            } else if let Item::Macro2(item_macro2) = item {
                macro_definitions.insert(item_macro2.ident.to_string());
            }
        }
        let mut visitor = MacroInvocationVisitor {
            macro_names,
            macro_definitions,
            async_context: false,
            macro_invocations: Vec::new(),
        };
        visitor.visit_file(file);
        let mut rewriter = MacroInvocationRewriter {
            target_invocations: visitor.macro_invocations,
        };
        rewriter.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macro invocations, especially within async contexts, by injecting semicolon-related lint attributes and ensuring proper semicolon placement. It modifies both macro definitions and their usage sites to provoke compiler lint warnings, aiming to explore the interaction between macros, semicolon handling, and lint diagnostics."
    }
}

struct MacroInvocationVisitor {
    macro_names: HashSet<String>,
    macro_definitions: HashSet<String>,
    async_context: bool,
    macro_invocations: Vec<proc_macro2::Span>,
}

impl<'ast> Visit<'ast> for MacroInvocationVisitor {
    fn visit_expr(&mut self, node: &'ast Expr) {
        if let Expr::Macro(expr_macro) = node {
            let path = &expr_macro.mac.path;
            if let Some(ident) = path.get_ident() {
                if self.macro_names.contains(&ident.to_string()) {
                    if self.async_context {
                        self.macro_invocations.push(expr_macro.mac.span());
                    }
                }
            }
        }
        match node {
            Expr::Async(expr_async) => {
                let was_async = self.async_context;
                self.async_context = true;
                syn::visit::visit_block(self, &expr_async.block);
                self.async_context = was_async;
            }
            Expr::Block(expr_block) => {
                syn::visit::visit_block(self, &expr_block.block);
            }
            Expr::Closure(expr_closure) => {
                syn::visit::visit_expr_closure(self, expr_closure);
            }
            _ => {
                syn::visit::visit_expr(self, node);
            }
        }
    }

    fn visit_item_fn(&mut self, node: &'ast ItemFn) {
        let was_async = self.async_context;
        self.async_context = node.sig.asyncness.is_some();
        syn::visit::visit_block(self, &node.block);
        self.async_context = was_async;
    }

    fn visit_item_impl(&mut self, node: &'ast ItemImpl) {
        syn::visit::visit_item_impl(self, node);
    }

    fn visit_item_trait(&mut self, node: &'ast ItemTrait) {
        syn::visit::visit_item_trait(self, node);
    }

    fn visit_impl_item(&mut self, node: &'ast ImplItem) {
        match node {
            ImplItem::Fn(item_fn) => {
                let was_async = self.async_context;
                self.async_context = item_fn.sig.asyncness.is_some();
                syn::visit::visit_block(self, &item_fn.block);
                self.async_context = was_async;
            }
            _ => {
                syn::visit::visit_impl_item(self, node);
            }
        }
    }

    fn visit_trait_item(&mut self, node: &'ast TraitItem) {
        match node {
            TraitItem::Fn(item_fn) => {
                if let Some(block) = &item_fn.default {
                    let was_async = self.async_context;
                    self.async_context = item_fn.sig.asyncness.is_some();
                    syn::visit::visit_block(self, block);
                    self.async_context = was_async;
                }
            }
            _ => {
                syn::visit::visit_trait_item(self, node);
            }
        }
    }

    fn visit_stmt(&mut self, node: &'ast Stmt) {
        match node {
            Stmt::Local(local) => {
                if let Some((_, expr)) = &local.init {
                    self.visit_expr(expr);
                }
            }
            Stmt::Item(item) => {
                self.visit_item(item);
            }
            Stmt::Expr(expr, _) => {
                self.visit_expr(expr);
            }
            Stmt::Macro(stmt_macro) => {
                let path = &stmt_macro.mac.path;
                if let Some(ident) = path.get_ident() {
                    if self.macro_names.contains(&ident.to_string()) {
                        if self.async_context {
                            self.macro_invocations.push(stmt_macro.mac.span());
                        }
                    }
                }
            }
            Stmt::Semi(expr, _) => {
                self.visit_expr(expr);
            }
        }
    }
}

struct MacroInvocationRewriter {
    target_invocations: Vec<proc_macro2::Span>,
}

impl VisitMut for MacroInvocationRewriter {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Macro(expr_macro) = node {
            if self.target_invocations.contains(&expr_macro.mac.span()) {
                let mut has_warn = false;
                let mut has_allow = false;
                let mut has_expect = false;
                let mut new_attrs = Vec::new();
                for attr in &expr_macro.attrs {
                    if attr.path().is_ident("warn") {
                        if let Ok(meta) = attr.parse_meta() {
                            if let Meta::List(meta_list) = meta {
                                if meta_list
                                    .parse_args_with(Punctuated::<Path, token::Comma>::parse_terminated)
                                    .map(|paths| {
                                        paths
                                            .iter()
                                            .any(|path| path.is_ident("semicolon_in_expressions_from_macros"))
                                    })
                                    .unwrap_or(false)
                                {
                                    has_warn = true;
                                }
                            }
                        }
                    } else if attr.path().is_ident("allow") {
                        if let Ok(meta) = attr.parse_meta() {
                            if let Meta::List(meta_list) = meta {
                                if meta_list
                                    .parse_args_with(Punctuated::<Path, token::Comma>::parse_terminated)
                                    .map(|paths| {
                                        paths
                                            .iter()
                                            .any(|path| path.is_ident("semicolon_in_expressions_from_macros"))
                                    })
                                    .unwrap_or(false)
                                {
                                    has_allow = true;
                                }
                            }
                        }
                    } else if attr.path().is_ident("expect") {
                        if let Ok(meta) = attr.parse_meta() {
                            if let Meta::List(meta_list) = meta {
                                if meta_list
                                    .parse_args_with(Punctuated::<Path, token::Comma>::parse_terminated)
                                    .map(|paths| {
                                        paths
                                            .iter()
                                            .any(|path| path.is_ident("semicolon_in_expressions_from_macros"))
                                    })
                                    .unwrap_or(false)
                                {
                                    has_expect = true;
                                }
                            }
                        }
                    }
                }
                for attr in &expr_macro.attrs {
                    if attr.path().is_ident("warn") {
                        new_attrs.push(attr.clone());
                    } else if attr.path().is_ident("allow") {
                        if !has_warn && !has_expect {
                            new_attrs.push(attr.clone());
                        }
                    } else if attr.path().is_ident("expect") {
                        if !has_warn && !has_allow {
                            new_attrs.push(attr.clone());
                        }
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }
                if !has_warn {
                    new_attrs.push(parse_quote! { #[warn(semicolon_in_expressions_from_macros)] });
                }
                expr_macro.attrs = new_attrs;
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }

    fn visit_stmt_mut(&mut self, node: &mut Stmt) {
        match node {
            Stmt::Local(local) => {
                if let Some((_, expr)) = &mut local.init {
                    self.visit_expr_mut(expr);
                }
            }
            Stmt::Item(item) => {
                self.visit_item_mut(item);
            }
            Stmt::Expr(expr, semi) => {
                self.visit_expr_mut(expr);
                if let Expr::Macro(expr_macro) = expr {
                    if self.target_invocations.contains(&expr_macro.mac.span()) {
                        *semi = Some(token::Semi::default());
                    }
                }
            }
            Stmt::Macro(stmt_macro) => {
                if self.target_invocations.contains(&stmt_macro.mac.span()) {
                    let mut has_warn = false;
                    let mut has_allow = false;
                    let mut has_expect = false;
                    let mut new_attrs = Vec::new();
                    for attr in &stmt_macro.attrs {
                        if attr.path().is_ident("warn") {
                            if let Ok(meta) = attr.parse_meta() {
                                if let Meta::List(meta_list) = meta {
                                    if meta_list
                                        .parse_args_with(Punctuated::<Path, token::Comma>::parse_terminated)
                                        .map(|paths| {
                                            paths
                                                .iter()
                                                .any(|path| path.is_ident("semicolon_in_expressions_from_macros"))
                                        })
                                        .unwrap_or(false)
                                    {
                                        has_warn = true;
                                    }
                                }
                            }
                        } else if attr.path().is_ident("allow") {
                            if let Ok(meta) = attr.parse_meta() {
                                if let Meta::List(meta_list) = meta {
                                    if meta_list
                                        .parse_args_with(Punctuated::<Path, token::Comma>::parse_terminated)
                                        .map(|paths| {
                                            paths
                                                .iter()
                                                .any(|path| path.is_ident("semicolon_in_expressions_from_macros"))
                                        })
                                        .unwrap_or(false)
                                    {
                                        has_allow = true;
                                    }
                                }
                            }
                        } else if attr.path().is_ident("expect") {
                            if let Ok(meta) = attr.parse_meta() {
                                if let Meta::List(meta_list) = meta {
                                    if meta_list
                                        .parse_args_with(Punctuated::<Path, token::Comma>::parse_terminated)
                                        .map(|paths| {
                                            paths
                                                .iter()
                                                .any(|path| path.is_ident("semicolon_in_expressions_from_macros"))
                                        })
                                        .unwrap_or(false)
                                    {
                                        has_expect = true;
                                    }
                                }
                            }
                        }
                    }
                    for attr in &stmt_macro.attrs {
                        if attr.path().is_ident("warn") {
                            new_attrs.push(attr.clone());
                        } else if attr.path().is_ident("allow") {
                            if !has_warn && !has_expect {
                                new_attrs.push(attr.clone());
                            }
                        } else if attr.path().is_ident("expect") {
                            if !has_warn && !has_allow {
                                new_attrs.push(attr.clone());
                            }
                        } else {
                            new_attrs.push(attr.clone());
                        }
                    }
                    if !has_warn {
                        new_attrs.push(parse_quote! { #[warn(semicolon_in_expressions_from_macros)] });
                    }
                    stmt_macro.attrs = new_attrs;
                    stmt_macro.semi_token = Some(token::Semi::default());
                }
            }
            Stmt::Semi(expr, _) => {
                self.visit_expr_mut(expr);
            }
        }
    }
}