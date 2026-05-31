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

pub struct Modify_Array_Length_In_Struct_17;

impl Mutator for Modify_Array_Length_In_Struct_17 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Struct_17"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let Expr::Lit(expr_lit) = &mut *type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                let current_length = lit_int.base10_parse::<usize>().unwrap();
                                let new_length = if current_length < 8 { 0 } else { current_length * 2 };
                                *type_array.len = parse_quote!(#new_length);
                            }
                        }
                    }
                }
            }

            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(Expr::Struct(expr_struct)) = stmt {
                                for field_value in &mut expr_struct.fields {
                                    if let Expr::Array(expr_array) = &mut field_value.expr {
                                        if let Some(Expr::Lit(ExprLit { lit: syn::Lit::Int(lit_int), .. })) = expr_array.elems.first() {
                                            let current_length = lit_int.base10_parse::<usize>().unwrap();
                                            let new_length = if current_length < 8 { 0 } else { current_length * 2 };
                                            expr_array.elems = Punctuated::from_iter((0..new_length).map(|_| parse_quote!(0)));
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
        "This mutator targets struct definitions with array fields initialized with constant lengths. It modifies the array length by either reducing it to zero or doubling it, depending on the initial size. This aims to challenge the compiler's handling of struct layouts and memory assumptions, potentially uncovering bugs related to array size changes."
    }
}