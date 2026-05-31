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

pub struct Mutate_Macro_Invocation_Statements_203;

impl Mutator for Mutate_Macro_Invocation_Statements_203 {
    fn name(&self) -> &str {
        "Mutate_Macro_Invocation_Statements_203"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                let macro_name = mac.mac.path.segments.last().unwrap().ident.to_string();
                if macro_name.contains("print") || macro_name.contains("assert") {
                    continue;
                }
                let mut new_tokens = proc_macro2::TokenStream::new();
                let mut tokens = mac.mac.tokens.clone().into_iter().peekable();
                while let Some(token) = tokens.next() {
                    if let proc_macro2::TokenTree::Ident(ident) = &token {
                        if let Some(proc_macro2::TokenTree::Punct(punct)) = tokens.peek() {
                            if punct.as_char() == ':' {
                                if let Some(proc_macro2::TokenTree::Ident(stmt_ident)) =
                                    tokens.nth(1)
                                {
                                    if stmt_ident == "stmt" {
                                        new_tokens.extend(quote! { $a:stmt });
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                    new_tokens.extend(quote! { #token });
                }
                mac.mac.tokens = new_tokens;
            }
        }
        struct MacroInvocationVisitor {
            invocations: Vec<syn::Macro>,
        }
        impl<'ast> Visit<'ast> for MacroInvocationVisitor {
            fn visit_macro(&mut self, node: &'ast syn::Macro) {
                self.invocations.push(node.clone());
            }
        }
        let mut visitor = MacroInvocationVisitor {
            invocations: Vec::new(),
        };
        visitor.visit_file(file);
        let mut rng = thread_rng();
        let mut selected_invocation = None;
        for invocation in &visitor.invocations {
            let macro_name = invocation.path.segments.last().unwrap().ident.to_string();
            if macro_name.contains("print") || macro_name.contains("assert") {
                continue;
            }
            selected_invocation = Some(invocation.clone());
            if rng.gen_bool(0.5) {
                break;
            }
        }
        if let Some(invocation) = selected_invocation {
            let mut new_tokens = proc_macro2::TokenStream::new();
            let mut tokens = invocation.tokens.clone().into_iter().peekable();
            let mut stmt_count = 0;
            while let Some(token) = tokens.next() {
                if let proc_macro2::TokenTree::Ident(ident) = &token {
                    if let Some(proc_macro2::TokenTree::Punct(punct)) = tokens.peek() {
                        if punct.as_char() == ':' {
                            if let Some(proc_macro2::TokenTree::Ident(stmt_ident)) = tokens.nth(1) {
                                if stmt_ident == "stmt" {
                                    stmt_count += 1;
                                    if stmt_count == 1 {
                                        new_tokens.extend(quote! { $b:stmt });
                                    } else {
                                        new_tokens.extend(quote! { $a:stmt });
                                    }
                                    continue;
                                }
                            }
                        }
                    }
                }
                new_tokens.extend(quote! { #token });
            }
            let new_invocation = syn::Macro {
                path: invocation.path.clone(),
                bang_token: invocation.bang_token,
                delimiter: invocation.delimiter.clone(),
                tokens: new_tokens,
            };
            let mut replaced = false;
            let mut new_items = Vec::new();
            for item in &file.items {
                if let Item::Macro(mac) = item {
                    if !replaced && mac.mac == invocation {
                        new_items.push(Item::Macro(ItemMacro {
                            attrs: mac.attrs.clone(),
                            mac: new_invocation.clone(),
                            semi_token: mac.semi_token,
                        }));
                        replaced = true;
                    } else {
                        new_items.push(item.clone());
                    }
                } else {
                    new_items.push(item.clone());
                }
            }
            file.items = new_items;
        }
        let mut rng = thread_rng();
        let mut selected_invocation = None;
        for (i, item) in file.items.iter().enumerate() {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                for (j, stmt) in item_fn.block.stmts.iter().enumerate() {
                    if let Stmt::Macro(mac) = stmt {
                        let macro_name = mac.mac.path.segments.last().unwrap().ident.to_string();
                        if macro_name.contains("print") || macro_name.contains("assert") {
                            continue;
                        }
                        selected_invocation = Some((i, j, mac.clone()));
                        if rng.gen_bool(0.5) {
                            break;
                        }
                    }
                }
            }
        }
        if let Some((i, j, invocation)) = selected_invocation {
            let mut new_tokens = proc_macro2::TokenStream::new();
            let mut tokens = invocation.mac.tokens.clone().into_iter().peekable();
            let mut stmt_count = 0;
            while let Some(token) = tokens.next() {
                if let proc_macro2::TokenTree::Group(group) = &token {
                    let mut inner_tokens = proc_macro2::TokenStream::new();
                    let mut inner_iter = group.stream().into_iter().peekable();
                    while let Some(inner_token) = inner_iter.next() {
                        if let proc_macro2::TokenTree::Ident(ident) = &inner_token {
                            if let Some(proc_macro2::TokenTree::Punct(punct)) = inner_iter.peek() {
                                if punct.as_char() == ':' {
                                    if let Some(proc_macro2::TokenTree::Ident(stmt_ident)) =
                                        inner_iter.nth(1)
                                    {
                                        if stmt_ident == "stmt" {
                                            stmt_count += 1;
                                            if stmt_count == 1 {
                                                inner_tokens.extend(quote! { $b:stmt });
                                            } else {
                                                inner_tokens.extend(quote! { $a:stmt });
                                            }
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                        inner_tokens.extend(quote! { #inner_token });
                    }
                    let new_group = proc_macro2::Group::new(group.delimiter(), inner_tokens);
                    new_tokens.extend(quote! { #new_group });
                } else {
                    new_tokens.extend(quote! { #token });
                }
            }
            let new_invocation = syn::Macro {
                path: invocation.mac.path.clone(),
                bang_token: invocation.mac.bang_token,
                delimiter: invocation.mac.delimiter.clone(),
                tokens: new_tokens,
            };
            if let Item::Fn(item_fn) = &mut file.items[i] {
                let mut new_stmts = Vec::new();
                for (k, stmt) in item_fn.block.stmts.iter().enumerate() {
                    if k == j {
                        new_stmts.push(Stmt::Macro(syn::StmtMacro {
                            attrs: invocation.attrs.clone(),
                            mac: new_invocation.clone(),
                            semi_token: invocation.semi_token,
                        }));
                    } else {
                        new_stmts.push(stmt.clone());
                    }
                }
                item_fn.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macro invocations that involve statement parameters. It first modifies the corresponding macro definition to use a specific parameter naming scheme ($a and $b for stmt parameters). Then, it selects a macro invocation within a function body and swaps the first and second statement arguments. This transformation alters the control flow or logic encapsulated by the macro, potentially revealing bugs in macro expansion, statement parsing, and control flow analysis."
    }
}