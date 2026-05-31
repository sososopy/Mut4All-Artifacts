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

pub struct Replace_ImplTrait_Infinite_Recursion_With_Unit_127;

impl Mutator for Replace_ImplTrait_Infinite_Recursion_With_Unit_127 {
    fn name(&self) -> &str {
        "Replace_ImplTrait_Infinite_Recursion_With_Unit_127"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        let body = &item_fn.block;
                        if body.stmts.len() == 1 {
                            let first_stmt = &body.stmts[0];
                            if let Stmt::Expr(expr, None) = first_stmt {
                                if let Expr::Call(call_expr) = expr {
                                    if let Expr::Path(path_expr) = &*call_expr.func {
                                        if path_expr.path.segments.len() == 1 {
                                            let segment = &path_expr.path.segments[0];
                                            if segment.ident == item_fn.sig.ident {
                                                item_fn.sig.output = ReturnType::Default;
                                                let new_expr: Expr = parse_quote! { () };
                                let mut new_block = item_fn.block.clone();
                                new_block.stmts.clear();
                                new_block.stmts.push(Stmt::Expr(new_expr, None));
                                new_block.stmts.push(Stmt::Expr(Expr::Call(call_expr.clone()), None));
                                item_fn.block = new_block;
                            }
                        }
                    }
                }
            }
        }
    }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait` that exhibit infinite recursion via a single recursive call. It replaces the return type with unit `()` while preserving the recursive call, aiming to bypass compiler assumptions about recursion in `impl Trait` contexts. This transformation may trigger internal compiler errors related to destructors or opaque type handling."
    }
}