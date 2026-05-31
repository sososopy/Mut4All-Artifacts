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

pub struct Replace_Array_Index_With_Maximum_Value_76;

impl Mutator for Replace_Array_Index_With_Maximum_Value_76 {
    fn name(&self) -> &str {
        "Replace_Array_Index_With_Maximum_Value_76"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Index(expr_index) = &**expr {
                            if let Expr::Path(path) = &*expr_index.expr {
                                if let Some(array_ident) = path.path.get_ident() {
                                    let array_len = quote! { #array_ident.len() - 1 };
                                    let new_expr = quote! { #array_ident[#array_len] };
                                    *expr = Box::new(new_expr);
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Index(expr_index) = &**expr {
                                    if let Expr::Path(path) = &*expr_index.expr {
                                        if let Some(array_ident) = path.path.get_ident() {
                                            let array_len = quote! { #array_ident.len() - 1 };
                                            let new_expr = quote! { #array_ident[#array_len] };
                                            *expr = Box::new(new_expr);
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
        "The mutation operator replaces array index operations with the maximum possible value for the array type. This transformation aims to test the compiler's handling of array indexing and bounds checking, potentially leading to bugs or errors in the compiler's implementation of these operations."
    }
}