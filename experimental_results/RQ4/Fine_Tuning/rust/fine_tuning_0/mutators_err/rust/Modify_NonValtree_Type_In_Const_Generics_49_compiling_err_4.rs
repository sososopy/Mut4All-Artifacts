use proc_macro2::{Span, Ident};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident as SynIdent,
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

pub struct Modify_NonValtree_Type_In_Const_Generics_49;

impl Mutator for Modify_NonValtree_Type_In_Const_Generics_49 {
    fn name(&self) -> &str {
        "Modify_NonValtree_Type_In_Const_Generics_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(TypePath { path, .. }) = &const_param.ty {
                            if path.is_ident("fn") {
                                return Some(const_param);
                            }
                        }
                    }
                    None
                }) {
                    // Change the const generic parameter type to a simple usize
                    generics.ty = parse_quote!(usize);
                    generics.default = Some(parse_quote!(1));

                    // Update any instance creation of the struct to reflect the new parameter change
                    let ident = &item_struct.ident;
                    for item in &mut file.items {
                        if let Item::Fn(item_fn) = item {
                            for stmt in &mut item_fn.block.stmts {
                                if let Stmt::Local(local) = stmt {
                                    if let Some(LocalInit { expr: init_expr, .. }) = &mut local.init {
                                        if let Expr::Path(expr_path) = &mut **init_expr {
                                            if expr_path.path.is_ident(ident) {
                                                expr_path.path.segments.last_mut().unwrap().arguments = PathArguments::None;
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
        "The mutation operator targets struct definitions with const generic parameters that use non-valtree-compatible types, such as functions. It modifies these parameters to use simple, valtree-compatible types like usize, and updates any instance creation to reflect this change. This tests the compiler's ability to handle const generics and evaluate const expressions correctly."
    }
}