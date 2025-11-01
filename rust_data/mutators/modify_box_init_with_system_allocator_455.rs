use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Modify_Box_Init_With_System_Allocator_455;

impl Mutator for Modify_Box_Init_With_System_Allocator_455 {
    fn name(&self) -> &str {
        "Modify_Box_Init_With_System_Allocator_455"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut uses_system = false;

        for item in &mut file.items {
            if let Item::Use(use_item) = item {
                let mut use_tree = &use_item.tree;
                while let syn::UseTree::Path(path) = use_tree {
                    if path.ident == "std" {
                        use_tree = &*path.tree;
                        if let syn::UseTree::Path(inner_path) = use_tree {
                            if inner_path.ident == "alloc" {
                                use_tree = &*inner_path.tree;
                                if let syn::UseTree::Name(name) = use_tree {
                                    if name.ident == "System" {
                                        uses_system = true;
                                        break;
                                    }
                                }
                            }
                        }
                    } else {
                        break;
                    }
                }
            }
        }

        if !uses_system {
            file.items.insert(0, parse_quote! { use std::alloc::System; });
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr: init_expr, .. }) = &mut local.init {
                            if let Expr::Call(expr_call) = &mut **init_expr {
                                if let Expr::Path(expr_path) = &*expr_call.func {
                                    if expr_path.path.segments.last().unwrap().ident == "new_in" {
                                        if let Some(Expr::Reference(alloc_expr)) = expr_call.args.last_mut() {
                                            if let Expr::Path(alloc_path) = &*alloc_expr.expr {
                                                if alloc_path.path.segments.last().unwrap().ident != "System" {
                                                    let new_expr: Expr = parse_quote! { &System };
                                                    expr_call.args.pop();
                                                    expr_call.args.push(new_expr);
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
        "The mutation operator targets Box initializations using the `Box::new_in` method with a custom allocator. It modifies these initializations to use `std::alloc::System` instead. If the seed program does not already include a `use std::alloc::System;` statement, it adds one. This transformation is designed to test the Rust compiler's handling of different allocator types and potentially reveal allocator-related bugs."
    }
}