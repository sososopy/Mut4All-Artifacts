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

pub struct Add_Conflicting_Attribute_To_Used_Function_197;

impl Mutator for Add_Conflicting_Attribute_To_Used_Function_197 {
    fn name(&self) -> &str {
        "Add_Conflicting_Attribute_To_Used_Function_197"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_function: Option<Ident> = None;
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.attrs.is_empty() {
                    target_function = Some(item_fn.sig.ident.clone());
                    let mut has_inline = false;
                    for attr in &item_fn.attrs {
                        if attr.path.is_ident("inline") {
                            has_inline = true;
                            break;
                        }
                    }
                    if !has_inline {
                        item_fn.attrs.push(parse_quote!(#[inline(always)]));
                    }
                }
            }
        }
        if let Some(target_ident) = target_function {
            let mut has_call = false;
            for item in &file.items {
                if let Item::Fn(item_fn) = item {
                    if item_fn.sig.ident == target_ident {
                        continue;
                    }
                    for stmt in &item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                            if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                                if path.is_ident(&target_ident) {
                                    has_call = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if !has_call {
                let call_expr: Expr = parse_quote! { #target_ident() };
                let call_stmt = Stmt::Expr(call_expr, None);
                let mut inserted = false;
                for item in &mut file.items {
                    if let Item::Fn(item_fn) = item {
                        if item_fn.sig.ident != target_ident {
                            item_fn.block.stmts.insert(0, call_stmt.clone());
                            inserted = true;
                            break;
                        }
                    }
                }
                if !inserted {
                    for item in &mut file.items {
                        if let Item::Fn(item_fn) = item {
                            if item_fn.sig.ident == target_ident {
                                item_fn.block.stmts.push(call_stmt.clone());
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify a function with attributes, add #[inline(always)] if not present, ensure the function is called in another function, and insert a call if missing."
    }
}