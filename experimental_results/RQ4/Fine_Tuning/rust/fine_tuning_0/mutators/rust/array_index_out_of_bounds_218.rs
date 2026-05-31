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

pub struct Array_Index_Out_Of_Bounds_218;

impl Mutator for Array_Index_Out_Of_Bounds_218 {
    fn name(&self) -> &str {
        "Array_Index_Out_Of_Bounds_218"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(item_const) = item {
                if let syn::Expr::Index(expr_index) = &*item_const.expr {
                    if let syn::Expr::Path(expr_path) = &*expr_index.expr {
                        if let Some(array_ident) = expr_path.path.get_ident() {
                            if let syn::Expr::Lit(expr_lit) = &*expr_index.index {
                                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                    let original_index = lit_int.base10_parse::<usize>().unwrap_or(0);
                                    let new_index = original_index + 1; // Intentionally out of bounds
                                    let new_expr: Expr = parse_quote! { #array_ident[#new_index] };
                                    item_const.expr = Box::new(new_expr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant definitions involving array indexing. It modifies the index to exceed the array's bounds, creating an intentional out-of-bounds error. This transformation aims to test the compiler's handling of constant evaluation and provoke ICEs during the MIR optimization process."
    }
}