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

pub struct Alter_Const_Evaluation_In_Trait_Impls_529;

impl Mutator for Alter_Const_Evaluation_In_Trait_Impls_529 {
    fn name(&self) -> &str {
        "Alter_Const_Evaluation_In_Trait_Impls_529"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if item_impl.generics.params.iter().any(|param| matches!(param, GenericParam::Const(_))) {
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Type(impl_type) = impl_item {
                            if let Some(expr) = extract_size_of_expr(&impl_type.ty) {
                                let new_expr: Expr = parse_quote! { (#expr + 0) };
                                replace_expr(&mut impl_type.ty, &expr, new_expr);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn extract_size_of_expr(ty: &Type) -> Option<Expr> {
    if let Type::Array(type_array) = ty {
        if let Expr::Call(expr_call) = &*type_array.len {
            if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                if path.is_ident("std::mem::size_of") {
                    return Some((**type_array.len).clone());
                }
            }
        }
    }
    None
}

fn replace_expr(ty: &mut Type, old_expr: &Expr, new_expr: Expr) {
    if let Type::Array(type_array) = ty {
        if *type_array.len == *old_expr {
            type_array.len = Box::new(new_expr);
        }
    }
}