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

pub struct Array_Init_And_Index_Modifier_17;

impl Mutator for Array_Init_And_Index_Modifier_17 {
    fn name(&self) -> &str {
        "Array_Init_And_Index_Modifier_17"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(fields) = &mut item_struct.fields {
                    for field in fields {
                        if let Type::Array(type_array) = &field.ty {
                            if let Expr::Lit(expr_lit) = &type_array.len {
                                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                    let new_size: usize = lit_int.base10_parse().unwrap_or(0) + 2;
                                    let new_expr_len: Expr = parse_quote! { #new_size };
                                    field.ty = Type::Array(TypeArray {
                                        bracket_token: type_array.bracket_token,
                                        elem: type_array.elem.clone(),
                                        len: Box::new(new_expr_len),
                                    });
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        let mut new_stmts = vec![];
                        for stmt in &func.block.stmts {
                            new_stmts.push(stmt.clone());
                            if let Stmt::Local(local) = stmt {
                                if let Some((_, expr)) = &local.init {
                                    if let Expr::Struct(expr_struct) = &**expr {
                                        for field in &expr_struct.fields {
                                            if let Expr::Array(expr_array) = &field.expr {
                                                let mut new_elements = expr_array.elems.clone();
                                                new_elements.push(parse_quote! { 0 });
                                                new_elements.push(parse_quote! { 1 });
                                                let new_expr_array = Expr::Array(ExprArray {
                                                    bracket_token: expr_array.bracket_token,
                                                    elems: new_elements,
                                                });
                                                let new_stmt: Stmt = parse_quote! {
                                                    let mut instance = #new_expr_array;
                                                    instance[4] = 10;
                                                    instance[5] = 20;
                                                };
                                                new_stmts.push(new_stmt);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        func.block.stmts = new_stmts;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with array fields, increasing the array size and modifying initialization to include out-of-bounds indexing. It aims to test the compiler's handling of struct layout and indexing operations, potentially exposing layout-related issues."
    }
}