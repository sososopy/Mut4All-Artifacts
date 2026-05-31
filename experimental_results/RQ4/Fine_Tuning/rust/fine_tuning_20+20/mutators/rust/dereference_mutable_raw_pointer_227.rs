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

pub struct Dereference_Mutable_Raw_Pointer_227;

impl Mutator for Dereference_Mutable_Raw_Pointer_227 {
    fn name(&self) -> &str {
        "Dereference_Mutable_Raw_Pointer_227"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut global_vars = HashSet::new();
        for item in &file.items {
            if let Item::Static(item_static) = item {
                if matches!(item_static.mutability, StaticMutability::Mut(_)) {
                    global_vars.insert(item_static.ident.to_string());
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut target_stmt_index = None;
                for (index, stmt) in item_fn.block.stmts.iter().enumerate() {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr, .. }) = &local.init {
                            if let Expr::Reference(expr_ref) = &**expr {
                                if expr_ref.mutability.is_some() {
                                    if let Expr::Path(expr_path) = &*expr_ref.expr {
                                        if expr_path
                                            .path
                                            .get_ident()
                                            .map(|ident| global_vars.contains(&ident.to_string()))
                                            .unwrap_or(false)
                                        {
                                            target_stmt_index = Some(index);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if let Some(index) = target_stmt_index {
                    let target_stmt = item_fn.block.stmts[index].clone();
                    if let Stmt::Local(local) = target_stmt {
                        if let Some(LocalInit { expr, .. }) = local.init {
                            if let Expr::Reference(expr_ref) = *expr {
                                if expr_ref.mutability.is_some() {
                                    let var_ident = match &local.pat {
                                        Pat::Ident(pat_ident) => pat_ident.ident.clone(),
                                        _ => Ident::new("_", Span::call_site()),
                                    };
                                    let deref_stmt: Stmt = parse_quote! {
                                        let _ = unsafe { *#var_ident };
                                    };
                                    item_fn
                                        .block
                                        .stmts
                                        .insert(index + 1, deref_stmt);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify mutable global variables and functions using them. Locate the creation of mutable raw pointers to these globals and inject a dereference operation to stress-test unsafe pointer handling."
    }
}