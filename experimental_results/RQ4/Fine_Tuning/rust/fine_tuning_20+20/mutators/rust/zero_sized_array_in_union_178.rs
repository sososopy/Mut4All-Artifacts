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

pub struct Zero_Sized_Array_In_Union_178;

impl Mutator for Zero_Sized_Array_In_Union_178 {
    fn name(&self) -> &str {
        "Zero_Sized_Array_In_Union_178"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                let mut has_zero_sized_array = false;
                let mut zero_sized_array_index = None;
                for (index, field) in item_union.fields.named.iter().enumerate() {
                    if let Type::Array(type_array) = &field.ty {
                        if let Expr::Lit(expr_lit) = &type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                if lit_int.base10_parse::<u64>().unwrap_or(1) == 0 {
                                    has_zero_sized_array = true;
                                    zero_sized_array_index = Some(index);
                                    break;
                                }
                            }
                        }
                    }
                }
                if has_zero_sized_array {
                    let mut new_fields = item_union.fields.named.clone();
                    if let Some(index) = zero_sized_array_index {
                        new_fields = new_fields.into_iter().enumerate().filter_map(|(i, field)| if i != index { Some(field) } else { None }).collect();
                    }
                    let replacement_field: syn::Field = parse_quote! { a: [u8; 1] };
                    new_fields.push(replacement_field);
                    item_union.fields.named = new_fields;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets unions containing zero-sized array fields, which are known to cause issues with Rust's layout and transmutability logic. By replacing zero-sized arrays with non-zero-sized arrays or other simple types, it forces the compiler to re-evaluate the union's layout and field access semantics, potentially uncovering bugs related to memory layout, type safety, and code generation."
    }
}