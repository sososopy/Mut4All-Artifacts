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

pub struct Modify_Constant_Block_With_Intrinsic_Function_425;

impl Mutator for Modify_Constant_Block_With_Intrinsic_Function_425 {
    fn name(&self) -> &str {
        "Modify_Constant_Block_With_Intrinsic_Function_425"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut found = false;
                for stmt in &func.block.stmts {
                    if let syn::Stmt::Item(syn::Item::Const(_)) = stmt {
                        found = true;
                        break;
                    }
                }
                if !found {
                    continue;
                }
                let mut candidates: Vec<&syn::Stmt> = func
                    .block
                    .stmts
                    .iter()
                    .filter(|stmt| {
                        if let syn::Stmt::Item(syn::Item::Const(_)) = stmt {
                            true
                        } else {
                            false
                        }
                    })
                    .collect();
                let mut rng = thread_rng();
                candidates.shuffle(&mut rng);
                if let Some(target_stmt) = candidates.first() {
                    let mut intrinsic_candidates = vec![
                        "core::intrinsics::volatile_load",
                        "core::intrinsics::volatile_store",
                        "core::intrinsics::copy",
                        "core::intrinsics::copy_nonoverlapping",
                        "core::intrinsics::write_bytes",
                        "core::intrinsics::transmute",
                        "core::intrinsics::likely",
                        "core::intrinsics::unlikely",
                    ];
                    if let syn::Stmt::Item(syn::Item::Const(item_const)) = target_stmt {
                        let mut found = false;
                        if let syn::Expr::Path(expr_path) = &*item_const.expr {
                            let path_str = expr_path
                                .path
                                .segments
                                .iter()
                                .map(|seg| seg.ident.to_string())
                                .collect::<Vec<_>>()
                                .join("::");
                            if intrinsic_candidates.contains(&path_str.as_str()) {
                                intrinsic_candidates.retain(|&x| x != path_str);
                                found = true;
                            }
                        }
                        if !found {
                            return;
                        }
                        intrinsic_candidates.shuffle(&mut rng);
                        if let Some(new_intrinsic) = intrinsic_candidates.first() {
                            let new_expr: syn::Expr = syn::parse_str(new_intrinsic).unwrap();
                            let new_stmt = syn::Stmt::Item(syn::Item::Const(syn::ItemConst {
                                attrs: item_const.attrs.clone(),
                                const_token: item_const.const_token,
                                ident: item_const.ident.clone(),
                                generics: item_const.generics.clone(),
                                colon_token: item_const.colon_token,
                                ty: item_const.ty.clone(),
                                eq_token: item_const.eq_token,
                                expr: Box::new(new_expr),
                                semi_token: item_const.semi_token,
                                vis: item_const.vis.clone(),
                            }));
                            let new_stmts: Vec<syn::Stmt> = func
                                .block
                                .stmts
                                .iter()
                                .map(|stmt| {
                                    if stmt == *target_stmt {
                                        new_stmt.clone()
                                    } else {
                                        stmt.clone()
                                    }
                                })
                                .collect();
                            func.block.stmts = new_stmts;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify functions with constant blocks using intrinsic functions. Replace the intrinsic function in one of these constant blocks with a different intrinsic function of similar type, ensuring the new function takes the same input and output types."
    }
}