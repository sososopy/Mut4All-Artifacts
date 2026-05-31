use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Target_Closure_With_Mutable_References_37;

impl Mutator for Target_Closure_With_Mutable_References_37 {
    fn name(&self) -> &str {
        "Target_Closure_With_Mutable_References_37"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut has_mut_self = false;
                for input in &func.sig.inputs {
                    if let syn::FnArg::Receiver(receiver) = input {
                        if receiver.mutability.is_some() {
                            has_mut_self = true;
                        }
                    }
                }
                if has_mut_self {
                    let mut new_stmts = Vec::new();
                    for (stmt_idx, stmt) in func.block.stmts.iter().enumerate() {
                        if let syn::Stmt::Local(local) = stmt {
                            if let Some(local_init) = &local.init {
                                if let syn::Expr::Closure(closure) = &*local_init.expr {
                                    let mut has_mut_self = false;
                                    for input in &closure.inputs {
                                        if let syn::Pat::Ident(pat_ident) = input {
                                            if pat_ident.ident == "self"
                                                && pat_ident.mutability.is_some()
                                            {
                                                has_mut_self = true;
                                            }
                                        }
                                    }
                                    if has_mut_self {
                                        let mut new_stmts = Vec::new();
                                        if let syn::Expr::Block(body) = &*closure.body {
                                            for (i, stmt) in body.block.stmts.iter().enumerate() {
                                                if i == 0 {
                                                    new_stmts.push(parse_quote! {
                                                        let _ = &self;
                                                    });
                                                }
                                                new_stmts.push(stmt.clone());
                                            }
                                            let new_body = syn::Block {
                                                brace_token: body.block.brace_token,
                                                stmts: new_stmts.clone(),
                                            };
                                            let new_closure = syn::ExprClosure {
                                                attrs: closure.attrs.clone(),
                                                asyncness: closure.asyncness,
                                                movability: closure.movability,
                                                capture: closure.capture,
                                                or1_token: closure.or1_token,
                                                inputs: closure.inputs.clone(),
                                                or2_token: closure.or2_token,
                                                output: closure.output.clone(),
                                                constness: closure.constness,
                                                lifetimes: closure.lifetimes.clone(),
                                                body: Box::new(syn::Expr::Block(syn::ExprBlock {
                                                    attrs: Vec::new(),
                                                    label: None,
                                                    block: new_body,
                                                })),
                                            };
                                            let new_local = syn::Local {
                                                attrs: local.attrs.clone(),
                                                let_token: local.let_token,
                                                pat: local.pat.clone(),
                                                init: Some(LocalInit {
                                                    eq_token: local_init.eq_token.clone(),
                                                    expr: Box::new(syn::Expr::Closure(new_closure)),
                                                    diverge: local_init.diverge.clone(),
                                                }),
                                                semi_token: local.semi_token,
                                            };
                                            new_stmts.push(syn::Stmt::Local(new_local));
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                        new_stmts.push(stmt.clone());
                    }
                    func.block.stmts = new_stmts;
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let mut has_mut_self = false;
                        for input in &method.sig.inputs {
                            if let syn::FnArg::Receiver(receiver) = input {
                                if receiver.mutability.is_some() {
                                    has_mut_self = true;
                                }
                            }
                        }
                        if has_mut_self {
                            let mut new_stmts = Vec::new();
                            for (stmt_idx, stmt) in method.block.stmts.iter().enumerate() {
                                if let syn::Stmt::Local(local) = stmt {
                                    if let Some(local_init) = &local.init {
                                        if let syn::Expr::Closure(closure) = &*local_init.expr {
                                            let mut has_mut_self = false;
                                            for input in &closure.inputs {
                                                if let syn::Pat::Ident(pat_ident) = input {
                                                    if pat_ident.ident == "self"
                                                        && pat_ident.mutability.is_some()
                                                    {
                                                        has_mut_self = true;
                                                    }
                                                }
                                            }
                                            if has_mut_self {
                                                let mut new_stmts = Vec::new();
                                                if let syn::Expr::Block(body) = &*closure.body {
                                                    for (i, stmt) in
                                                        body.block.stmts.iter().enumerate()
                                                    {
                                                        if i == 0 {
                                                            new_stmts.push(parse_quote! {
                                                                let _ = &self;
                                                            });
                                                        }
                                                        new_stmts.push(stmt.clone());
                                                    }
                                                    let new_body = syn::Block {
                                                        brace_token: body.block.brace_token,
                                                        stmts: new_stmts.clone(),
                                                    };
                                                    let new_closure = syn::ExprClosure {
                                                        attrs: closure.attrs.clone(),
                                                        asyncness: closure.asyncness,
                                                        movability: closure.movability,
                                                        capture: closure.capture,
                                                        or1_token: closure.or1_token,
                                                        inputs: closure.inputs.clone(),
                                                        or2_token: closure.or2_token,
                                                        output: closure.output.clone(),
                                                        constness: closure.constness,
                                                        lifetimes: closure.lifetimes.clone(),
                                                        body: Box::new(syn::Expr::Block(
                                                            syn::ExprBlock {
                                                                attrs: Vec::new(),
                                                                label: None,
                                                                block: new_body,
                                                            },
                                                        )),
                                                    };
                                                    let new_local = syn::Local {
                                                        attrs: local.attrs.clone(),
                                                        let_token: local.let_token,
                                                        pat: local.pat.clone(),
                                                        init: Some(LocalInit {
                                                            eq_token: local_init.eq_token.clone(),
                                                            expr: Box::new(syn::Expr::Closure(
                                                                new_closure,
                                                            )),
                                                            diverge: local_init.diverge.clone(),
                                                        }),
                                                        semi_token: local.semi_token,
                                                    };
                                                    new_stmts.push(syn::Stmt::Local(new_local));
                                                    continue;
                                                }
                                            }
                                        }
                                    }
                                }
                                new_stmts.push(stmt.clone());
                            }
                            method.block.stmts = new_stmts;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets closures within functions or methods that capture a mutable reference to `self`. It injects an immutable borrow of `self` at the beginning of the closure body, creating a conflict with the existing mutable borrow. This transformation stresses the borrow checker by introducing a borrow conflict within the closure's scope, potentially revealing issues in borrow analysis and lifetime resolution."
    }
}