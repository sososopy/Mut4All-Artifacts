use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Member, Pat, PatType, Path as SynPath,
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

pub struct Adjust_Array_Length_In_Struct_Initialization_17;

impl Mutator for Adjust_Array_Length_In_Struct_Initialization_17 {
    fn name(&self) -> &str {
        "Adjust_Array_Length_In_Struct_Initialization_17"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &item_struct.fields {
                    if let Type::Array(type_array) = &field.ty {
                        if let Expr::Lit(expr_lit) = &type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                let original_len = lit_int.base10_parse::<usize>().unwrap();
                                let new_len = if original_len > 1 {
                                    original_len - 1
                                } else {
                                    original_len + 1
                                };
                                let new_len_expr: Expr = parse_quote! { #new_len };
                                for item in &mut file.items {
                                    if let Item::Impl(item_impl) = item {
                                        for impl_item in &mut item_impl.items {
                                            if let ImplItem::Fn(method) = impl_item {
                                                if let Stmt::Expr(Expr::Struct(expr_struct)) = &mut method.block.stmts[0] {
                                                    for field_value in &mut expr_struct.fields {
                                                        if let Member::Named(ident) = &field_value.member {
                                                            if Some(ident) == field.ident.as_ref() {
                                                                if let Expr::Array(expr_array) = &mut field_value.expr {
                                                                    expr_array.elems.push(parse_quote! { 0 });
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
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}