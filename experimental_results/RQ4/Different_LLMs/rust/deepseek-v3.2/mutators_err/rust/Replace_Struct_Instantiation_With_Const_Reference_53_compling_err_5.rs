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

pub struct Replace_Struct_Instantiation_With_Const_Reference_53;

impl Mutator for Replace_Struct_Instantiation_With_Const_Reference_53 {
    fn name(&self) -> &str {
        "Replace_Struct_Instantiation_With_Const_Reference_53"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_defs = Vec::new();
        let mut const_items = Vec::new();
        let mut struct_instantiations = Vec::new();

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &item_struct.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.segments.last().map(|seg| seg.ident.to_string()) == Some("Copy".to_string()) {
                                    struct_defs.push(item_struct.clone());
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Const(item_const) = item {
                if let Type::Path(type_path) = &*item_const.ty {
                    if let Some(first_seg) = type_path.path.segments.first() {
                        for struct_def in &struct_defs {
                            if first_seg.ident == struct_def.ident {
                                const_items.push(item_const.clone());
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                struct_instantiations.extend(find_struct_instantiations(&item_fn.block));
            }
        }

        if struct_instantiations.is_empty() || const_items.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let target_instantiation = struct_instantiations.choose(&mut rng).unwrap();
        let target_const = const_items.choose(&mut rng).unwrap();

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                replace_struct_instantiation(&mut item_fn.block, target_instantiation, &target_const.ident);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a struct instantiation expression with a reference to a const item that initializes the same struct with mismatched generic trait bounds. It targets structs with Copy trait bounds and finds const items that instantiate those structs with unsized types (like slices). By substituting direct instantiation with a constant reference, it creates a mismatch between the expected Copy bound and the actual unsized type, potentially triggering const evaluation errors and layout mismatches during compilation."
    }
}

fn find_struct_instantiations(block: &syn::Block) -> Vec<syn::Expr> {
    let mut instantiations = Vec::new();
    for stmt in &block.stmts {
        if let Stmt::Expr(expr, _) = stmt {
            collect_struct_instantiations(expr, &mut instantiations);
        }
    }
    instantiations
}

fn collect_struct_instantiations(expr: &syn::Expr, instantiations: &mut Vec<syn::Expr>) {
    match expr {
        Expr::Call(call_expr) => {
            if let Expr::Path(path_expr) = &*call_expr.func {
                instantiations.push(expr.clone());
            }
        }
        Expr::Struct(struct_expr) => {
            instantiations.push(expr.clone());
        }
        _ => {
            if let Expr::Block(block_expr) = expr {
                for stmt in &block_expr.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        collect_struct_instantiations(expr, instantiations);
                    }
                }
            }
        }
    }
}

fn replace_struct_instantiation(block: &mut syn::Block, target: &syn::Expr, const_name: &syn::Ident) {
    for stmt in &mut block.stmts {
        if let Stmt::Expr(expr, _) = stmt {
            if *expr == *target {
                *expr = parse_quote! { #const_name };
            }
        }
    }
}