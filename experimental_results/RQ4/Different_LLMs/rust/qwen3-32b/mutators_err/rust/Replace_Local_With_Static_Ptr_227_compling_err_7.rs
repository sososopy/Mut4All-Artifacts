use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    *,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprIf, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemStruct, ItemStatic, Lifetime, LifetimeParam, Local, Pat, PatIdent,
    PatType, Path as SynPath, PathArguments, ReturnType, Type, TypeImplTrait, TypePath, parse_quote,
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

pub struct Replace_Local_With_Static_Ptr_227;

impl Mutator for Replace_Local_With_Static_Ptr_227 {
    fn name(&self) -> &str {
        "Replace_Local_With_Static_Ptr_227"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let syn::Pat::Type(pat_type) = local.pat.as_mut() {
                                let var_ident = match &*pat_type.pat {
                                    Pat::Ident(pat_ident) => &pat_ident.ident,
                                    _ => continue,
                                };
                                let static_ident = Ident::new(
                                    &var_ident.to_string().to_uppercase(),
                                    var_ident.span(),
                                );
                                let static_item = ItemStatic {
                                    vis: parse_quote!(pub),
                                    static_token: Default::default(),
                                    mutability: syn::StaticMutability::Mut(Default::default()),
                                    ident: static_ident.clone(),
                                    colon_token: Default::default(),
                                    ty: pat_type.ty.clone(),
                                    eq_token: Default::default(),
                                    expr: init.expr.clone(),
                                    semi_token: Default::default(),
                                    attrs: vec![],
                                };
                                file.items.push(Item::Static(static_item));
                                let ptr_ident = Ident::new(
                                    &format!("{}_ptr", var_ident),
                                    Span::call_site(),
                                );
                                let ptr_type = parse_quote!(*mut #pat_type.ty);
                                let raw_expr = parse_quote!(&raw mut #static_ident);
                                let new_local = Local {
                                    let_token: Default::default(),
                                    pat: parse_quote!(#ptr_ident: #ptr_type),
                                    init: Some(syn::LocalInit {
                                        eq_token: Default::default(),
                                        expr: Box::new(raw_expr),
                                        diverge: Default::default(),
                                    }),
                                    semi_token: parse_quote!(;),
                                    attrs: vec![],
                                };
                                *stmt = Stmt::Local(new_local);
                                for stmt in &mut func.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::If(expr_if) = *expr {
                                            if is_expr_uses_ident(&expr_if.cond, var_ident) {
                                                let new_cond = parse_quote!(!#ptr_ident.is_null());
                                                expr_if.cond = Box::new(new_cond);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a local variable with a static mut variable and accesses it through a raw pointer in a conditional check. This transformation introduces raw pointers to static mut variables and replaces the original condition with a null check, mimicking scenarios where scalar values are expected but raw pointers are used, potentially exposing MIR optimization bugs related to pointer handling and scalar expectations."
    }
}

fn is_expr_uses_ident(expr: &Expr, ident: &Ident) -> bool {
    let mut visitor = IdentVisitor { ident: ident.clone(), found: false };
    visitor.visit_expr(expr);
    visitor.found
}

struct IdentVisitor {
    ident: Ident,
    found: bool,
}

impl<'a> Visit<'a> for IdentVisitor {
    fn visit_ident(&mut self, i: &'a Ident) {
        if i.to_string() == self.ident.to_string() {
            self.found = true;
        }
    }
}