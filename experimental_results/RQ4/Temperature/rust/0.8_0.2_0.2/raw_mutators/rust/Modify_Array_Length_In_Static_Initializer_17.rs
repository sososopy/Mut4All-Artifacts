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

pub struct Modify_Array_Length_In_Static_Initializer_17;

impl Mutator for Modify_Array_Length_In_Static_Initializer_17 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Static_Initializer_17"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let Expr::Lit(expr_lit) = &type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                let current_size: usize = lit_int.base10_parse().unwrap();
                                let new_size = if current_size > 1 {
                                    current_size - 1
                                } else {
                                    current_size + 1
                                };
                                let new_size_expr: Expr = parse_quote! { #new_size };
                                type_array.len = new_size_expr;
                            }
                        }
                    }
                }
            }
            if let Item::Static(item_static) = item {
                if let Expr::Struct(expr_struct) = &mut *item_static.expr {
                    for field in &mut expr_struct.fields {
                        if let Expr::Array(expr_array) = &mut field.expr {
                            let current_length = expr_array.elems.len();
                            let new_length = if current_length > 1 {
                                current_length - 1
                            } else {
                                current_length + 1
                            };
                            if new_length > current_length {
                                expr_array.elems.push(parse_quote! { 0 });
                            } else {
                                expr_array.elems.pop();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets array declarations within static initializers in structs. It modifies the declared size of the array by either decreasing it by one (if the size is greater than one) or increasing it by one. This transformation aims to uncover issues related to layout computation and size constraints, particularly in scenarios involving static mutability and constant evaluation."
    }
}