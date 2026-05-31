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

pub struct Modify_Return_Path_In_Function_475;

impl Mutator for Modify_Return_Path_In_Function_475 {
    fn name(&self) -> &str {
        "Modify_Return_Path_In_Function_475"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut visitor = ConditionalReturnVisitor::new();
                        visitor.visit_block_mut(&mut item_fn.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets functions with `impl Trait` return types that use conditional logic to determine their return value. It modifies the else branch of the return path to return a boxed value, potentially altering the return type semantics and exploiting the compiler's handling of impl trait inference and type consistency."
    }
}

struct ConditionalReturnVisitor {
    modified: bool,
}

impl ConditionalReturnVisitor {
    fn new() -> Self {
        Self { modified: false }
    }
}

impl VisitMut for ConditionalReturnVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if self.modified {
            return;
        }
        if let Expr::If(expr_if) = expr {
            if let Some((_, else_block)) = &mut expr_if.else_branch {
                let else_expr = if else_block.stmts.len() == 1 {
                    if let Stmt::Expr(expr, _) = &else_block.stmts[0] {
                        expr.clone()
                    } else {
                        return;
                    }
                } else {
                    return;
                };
                let boxed_else_expr: Expr = parse_quote! { Box::new(#else_expr) };
                else_block.stmts = vec![Stmt::Expr(boxed_else_expr, None)];
                self.modified = true;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}