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

pub struct Replace_Index_With_Out_Of_Bounds_219;

impl Mutator for Replace_Index_With_Out_Of_Bounds_219 {
    fn name(&self) -> &str {
        "Replace_Index_With_Out_Of_Bounds_219"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = IndexVisitor {
            mutations: Vec::new(),
        };
        visitor.visit_file_mut(file);
        for (expr_index, new_index) in visitor.mutations {
            *expr_index = new_index;
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array/slice indexing expressions where the index is computed using a method call on the same variable (e.g., `len()`). It replaces the computed index with a constant out-of-bounds value: either `-1` or `var.len()`. The transformation directly substitutes the index part of the indexing operation, creating potential out-of-bounds access scenarios. This tests the compiler's bounds checking, constant evaluation, and error reporting for invalid indices, especially when the index expression involves method calls on the indexed variable."
    }
}

struct IndexVisitor {
    mutations: Vec<(Box<syn::Expr>, Box<syn::Expr>)>,
}

impl VisitMut for IndexVisitor {
    fn visit_expr_mut(&mut self, node: &mut syn::Expr) {
        if let syn::Expr::Index(expr_index) = node {
            if let syn::Expr::Path(expr_path) = &*expr_index.expr {
                if let Some(ident) = expr_path.path.get_ident() {
                    let var_name = ident.to_string();
                    match &*expr_index.index {
                        syn::Expr::Binary(expr_bin) => {
                            if let syn::Expr::MethodCall(method_call) = &*expr_bin.left {
                                if let syn::Expr::Path(receiver_path) = &*method_call.receiver {
                                    if let Some(receiver_ident) = receiver_path.path.get_ident() {
                                        if receiver_ident.to_string() == var_name && method_call.method.to_string() == "len" && method_call.args.is_empty() {
                                            if let syn::BinOp::Sub(_) = expr_bin.op {
                                                if let syn::Expr::Lit(expr_lit) = &*expr_bin.right {
                                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                        let k = lit_int.base10_parse::<i64>().unwrap_or(0);
                                                        let new_index: syn::Expr = if k > 0 {
                                                            parse_quote! { #ident.len() - 1 - #k }
                                                        } else {
                                                            parse_quote! { #ident.len() }
                                                        };
                                                        self.mutations.push((expr_index.index.clone(), Box::new(new_index)));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        syn::Expr::MethodCall(method_call) => {
                            if let syn::Expr::Path(receiver_path) = &*method_call.receiver {
                                if let Some(receiver_ident) = receiver_path.path.get_ident() {
                                    if receiver_ident.to_string() == var_name && method_call.method.to_string() == "len" && method_call.args.is_empty() {
                                        let new_index: syn::Expr = parse_quote! { -1 };
                                        self.mutations.push((expr_index.index.clone(), Box::new(new_index)));
                                    }
                                }
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}