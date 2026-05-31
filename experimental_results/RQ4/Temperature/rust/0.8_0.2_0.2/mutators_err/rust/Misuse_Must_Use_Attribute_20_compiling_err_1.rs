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

pub struct Misuse_Must_Use_Attribute_20;

impl Mutator for Misuse_Must_Use_Attribute_20 {
    fn name(&self) -> &str {
        "Misuse_Must_Use_Attribute_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.attrs.iter().any(|attr| attr.path.is_ident("must_use")) {
                    let mut new_stmts = Vec::new();
                    for stmt in &func.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some((_, expr)) = &local.init {
                                if let Expr::Call(call) = &**expr {
                                    if let Expr::Path(ExprPath { path, .. }) = &*call.func {
                                        if path.segments.last().unwrap().ident == func.sig.ident {
                                            // Ignore the result of the function call
                                            new_stmts.push(Stmt::Expr(Expr::Call(call.clone())));
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions annotated with #[must_use]. It modifies the function body to ignore the result of calls to the function itself, potentially revealing issues with the enforcement of the must-use attribute. This transformation aims to test the compiler's handling of must-use diagnostics in scenarios where the function's result is not utilized."
    }
}