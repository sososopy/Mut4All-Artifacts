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

pub struct Array_Index_Overflow_46;

impl Mutator for Array_Index_Overflow_46 {
    fn name(&self) -> &str {
        "Array_Index_Overflow_46"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct UnsafeArrayVisitor;

        impl VisitMut for UnsafeArrayVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Index(expr_index) = expr {
                    if let Expr::Lit(expr_lit) = &*expr_index.index {
                        if let syn::Lit::Int(_) = &expr_lit.lit {
                            let new_index: Expr = parse_quote!(usize::MAX - 1);
                            expr_index.index = Box::new(new_index);
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Unsafe(expr_unsafe)) = stmt {
                            for expr in &mut expr_unsafe.block.stmts {
                                if let Stmt::Expr(ref mut inner_expr) = expr {
                                    UnsafeArrayVisitor.visit_expr_mut(inner_expr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array index access within unsafe blocks inside constant functions or contexts. It modifies the index to `usize::MAX - 1` to test for overflow conditions, ensuring the access occurs within an unsafe block to bypass safety checks."
    }
}