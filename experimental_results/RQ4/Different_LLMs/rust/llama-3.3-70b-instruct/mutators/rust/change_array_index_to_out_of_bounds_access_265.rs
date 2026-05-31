use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Change_Array_Index_To_Out_Of_Bounds_Access_265;

impl Mutator for Change_Array_Index_To_Out_Of_Bounds_Access_265 {
    fn name(&self) -> &str {
        "Change_Array_Index_To_Out_Of_Bounds_Access_265"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_block = func.block.clone();
                for stmt in &mut new_block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Index(expr_index) = expr {
                            if let Expr::Path(path) = &*expr_index.expr {
                                if let Some(segment) = path.path.segments.last() {
                                    if segment.ident == "len" {
                                        // Replace the index with an out-of-bounds access
                                        let new_index = parse_quote! { #expr_index.expr[#expr_index.expr.len() + 1] };
                                        *expr = new_index;
                                    }
                                }
                            }
                        }
                    }
                }
                func.block = new_block;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes array index accesses to out-of-bounds accesses by adding 1 to the length of the array. This transformation aims to test the compiler's handling of array indexing and bounds checking, potentially triggering index out-of-bounds bugs."
    }
}