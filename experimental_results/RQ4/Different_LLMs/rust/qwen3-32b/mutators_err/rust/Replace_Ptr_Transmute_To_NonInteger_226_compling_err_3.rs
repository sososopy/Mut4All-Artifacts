use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, parse_quote,
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

pub struct Replace_Ptr_Transmute_To_NonInteger_226;

impl Mutator for Replace_Ptr_Transmute_To_NonInteger_226 {
    fn name(&self) -> &str {
        "Replace_Ptr_Transmute_To_NonInteger_226"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                process_function(&mut item_fn.block);
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        process_function(&mut method.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies transmutations from pointers to integer types within unsafe blocks and replaces the target scalar type with non-integer variants like f64, bool, or char. This disrupts MIR optimizations expecting integer scalars, potentially exposing assertion failures or incorrect optimizations in the compiler's handling of transmuted scalar types."
    }
}

fn process_function(block: &mut syn::Block) {
    for stmt in &mut block.stmts {
        if let Stmt::Local(local) = stmt {
            if let Some(init_expr) = &local.init {
                if let Expr::Unsafe(unsafe_expr) = &*init_expr.expr {
                    for stmt_in_block in &unsafe_expr.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt_in_block {
                            if let Expr::Call(call_expr) = &*expr {
                                if let Expr::Path(path_expr) = &*call_expr.func {
                                    let path = &path_expr.path;
                                    if path.segments.len() >= 2 {
                                        let first = path.segments[0].ident.to_string();
                                        let second = path.segments[1].ident.to_string();
                                        if first == "mem" && second == "transmute" {
                                            if let Pat::Type(pat_type) = &mut local.pat {
                                                if is_integer_type(&pat_type.ty) {
                                                    let new_type = choose_non_integer_type();
                                                    pat_type.ty = Box::new(new_type);
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

fn is_integer_type(ty: &syn::Type) -> bool {
    if let syn::Type::Path(type_path) = ty {
        let segments = &type_path.path.segments;
        if let Some(last) = segments.last() {
            let ident = last.ident.to_string();
            return ["usize", "isize", "u8", "u16", "u32", "u64", "u128", "i8", "i16", "i32", "i64", "i128"]
                .contains(&ident.as_str());
        }
    }
    false
}

fn choose_non_integer_type() -> syn::Type {
    let types = ["f64", "bool", "char"];
    let idx = rand::thread_rng().gen_range(0..types.len());
    let ty_str = types[idx];
    parse_quote!(#ty_str)
}