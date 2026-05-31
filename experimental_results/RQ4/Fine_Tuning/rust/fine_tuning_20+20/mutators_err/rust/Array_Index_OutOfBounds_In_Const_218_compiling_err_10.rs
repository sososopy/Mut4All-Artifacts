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

pub struct Array_Index_OutOfBounds_In_Const_218;

impl Mutator for Array_Index_OutOfBounds_In_Const_218 {
    fn name(&self) -> &str {
        "Array_Index_OutOfBounds_In_Const_218"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Expr::Index(expr_index) = &*item_const.expr {
                    if let Expr::Path(array_path) = &*expr_index.expr {
                        if let Some(array_ident) = array_path.path.get_ident() {
                            if let Some(array_item) = file
                                .items
                                .iter()
                                .find(|item| match item {
                                    Item::Const(item_const) => {
                                        item_const.ident == *array_ident
                                    }
                                    _ => false,
                                })
                            {
                                if let Item::Const(array_const) = array_item {
                                    if let Expr::Array(array_expr) = &*array_const.expr {
                                        let array_len = array_expr.elems.len();
                                        let new_index: Expr = parse_quote! { #array_len };
                                        expr_index.index = Box::new(new_index);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Const(impl_const) = impl_item {
                        if let Expr::Index(expr_index) = &*impl_const.expr {
                            if let Expr::Path(array_path) = &*expr_index.expr {
                                if let Some(array_ident) = array_path.path.get_ident() {
                                    if let Some(array_item) = item_impl
                                        .items
                                        .iter()
                                        .find(|impl_item| match impl_item {
                                            ImplItem::Const(impl_const) => {
                                                impl_const.ident == *array_ident
                                            }
                                            _ => false,
                                        })
                                    {
                                        if let ImplItem::Const(array_const) = array_item {
                                            if let Expr::Array(array_expr) = &*array_const.expr {
                                                let array_len = array_expr.elems.len();
                                                let new_index: Expr = parse_quote! { #array_len };
                                                expr_index.index = Box::new(new_index);
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
        "This mutator targets constant definitions that use array indexing, both at the top level and within impl blocks. It identifies the array being indexed and modifies the index to be out of bounds, specifically setting it to the array's length, which is one past the last valid index. This transformation exploits the ICE reported in the Rust compiler's MIR optimization phase, where out-of-bounds array accesses in constants can lead to internal errors during compile-time evaluation. By focusing on constant expressions, the mutator maximizes its impact across a wide range of Rust programs, particularly those utilizing const arrays and impl blocks, making it an effective tool for stress-testing the compiler's constant evaluation and MIR handling logic."
    }
}