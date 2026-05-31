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

pub struct Introduce_Mutable_Refs_In_Box_1;

impl Mutator for Introduce_Mutable_Refs_In_Box_1 {
    fn name(&self) -> &str {
        "Introduce_Mutable_Refs_In_Box_1"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let syn::Stmt::Expr(expr) = stmt {
                        if let syn::Expr::Call(expr_call) = expr {
                            if let syn::Expr::Path(expr_path) = &*expr_call.func {
                                if expr_path.path.is_ident("Box::new_in") {
                                    if let Some(syn::Expr::Reference(ref_expr)) = expr_call.args.first_mut() {
                                        if let syn::Expr::Array(array) = &*ref_expr.expr {
                                            let mut_expr: Expr = parse_quote! { &mut #array };
                                            *ref_expr.expr = Box::new(mut_expr);
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
        "The mutation operator targets function calls using `Box::new_in` with an allocator argument. It introduces mutable references to the data being boxed, simulating potential allocator mismanagement. This transformation aims to uncover bugs related to memory allocation and mutable aliasing by challenging the Rust compiler's handling of unsafe memory operations."
    }
}