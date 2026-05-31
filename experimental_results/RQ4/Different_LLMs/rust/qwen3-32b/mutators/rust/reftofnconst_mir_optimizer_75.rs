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

pub struct RefToFnConst_MIR_Optimizer_75;

impl Mutator for RefToFnConst_MIR_Optimizer_75 {
    fn name(&self) -> &str {
        "RefToFnConst_MIR_Optimizer_75"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_items = Vec::new();
        {
            for item in &file.items {
                if let syn::Item::Const(const_item) = item {
                    if let Type::Ptr(ptr) = &*const_item.ty {
                        if let Expr::Closure(_) = &*const_item.expr {
                            const_items.push((const_item.ident.to_string(), const_item.span()));
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let has_ref = check_function_for_const_references(func, &const_items);
                if has_ref {
                    func.attrs.push(parse_quote!(#[inline]));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const items of function pointer type assigned with closures and functions that reference them. It adds #[inline] attributes to those functions, increasing MIR optimization pressure and potentially exposing bugs in const evaluation and incremental compilation scenarios."
    }
}

fn check_function_for_const_references(func: &ItemFn, const_items: &[(String, Span)]) -> bool {
    for stmt in &func.block.stmts {
        if let Stmt::Local(local) = stmt {
            if let Some(init) = &local.init {
                if let Expr::Reference(expr_ref) = &*init.expr {
                    if let Expr::Path(expr_path) = &*expr_ref.expr {
                        for (const_name, _) in const_items {
                            if let Some(first_segment) = expr_path.path.segments.first() {
                                if first_segment.ident.to_string() == *const_name {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    false
}