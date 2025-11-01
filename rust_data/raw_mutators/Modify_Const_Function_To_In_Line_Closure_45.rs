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

pub struct Modify_Const_Function_To_In_Line_Closure_45;

impl Mutator for Modify_Const_Function_To_In_Line_Closure_45 {
    fn name(&self) -> &str {
        "Modify_Const_Function_To_In_Line_Closure_45"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_fn_map = HashMap::new();

        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Type::Path(type_path) = &*item_const.ty {
                    if type_path.path.segments.last().unwrap().ident == "fn" {
                        if let Expr::Closure(closure) = &*item_const.expr {
                            const_fn_map.insert(item_const.ident.clone(), closure.clone());
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &local.init {
                            if let Expr::Reference(expr_ref) = &**expr {
                                if let Expr::Path(expr_path) = &*expr_ref.expr {
                                    if let Some(ident) = expr_path.path.get_ident() {
                                        if let Some(closure) = const_fn_map.get(ident) {
                                            *stmt = Stmt::Local(Local {
                                                pat: local.pat.clone(),
                                                init: Some((local.init.as_ref().unwrap().0.clone(), Box::new(Expr::Closure(closure.clone())))),
                                                ..local.clone()
                                            });
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
        "The mutation operator targets const fn declarations returning closures. It identifies const fn items, extracts the closure, and maps these to their identifiers. When a reference to such a const fn is found in a function, it replaces the reference with the inlined closure, ensuring the logic remains intact while removing the const fn declaration."
    }
}