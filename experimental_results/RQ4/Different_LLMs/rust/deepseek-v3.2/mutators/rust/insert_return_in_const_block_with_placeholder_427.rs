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

pub struct Insert_Return_In_Const_Block_With_Placeholder_427;

impl Mutator for Insert_Return_In_Const_Block_With_Placeholder_427 {
    fn name(&self) -> &str {
        "Insert_Return_In_Const_Block_With_Placeholder_427"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Infer(_) = &**return_type {
                        let mut visitor = ConstBlockVisitor {
                            modified: false,
                        };
                        visitor.visit_block_mut(&mut item_fn.block);
                        if !visitor.modified {
                            let existing_expr = if item_fn.block.stmts.len() == 1 {
                                match &item_fn.block.stmts[0] {
                                    Stmt::Expr(expr, None) => Some(expr.clone()),
                                    _ => None,
                                }
                            } else {
                                None
                            };
                            let new_const_block: Expr = if let Some(expr) = existing_expr {
                                parse_quote! { const { #expr } }
                            } else {
                                let block_stmts = item_fn.block.stmts.clone();
                                parse_quote! { const { #(#block_stmts)* } }
                            };
                            let mutated_const_block: Expr = parse_quote! { const { ({ return (); }) } };
                            item_fn.block.stmts.clear();
                            item_fn.block.stmts.push(Stmt::Expr(parse_quote! { #mutated_const_block }, None));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with inferred return types using placeholder `_`. It modifies const blocks within these functions by adding an inner block expression wrapped in unnecessary parentheses and inserting a return statement returning unit `()` inside that inner block. If no const block exists, it wraps the function's existing body in a const block and applies the mutation. This transformation creates nested const contexts with redundant parentheses and early returns, testing the compiler's handling of const evaluation, block expression semantics, and return type inference for placeholder types."
    }
}

struct ConstBlockVisitor {
    modified: bool,
}

impl VisitMut for ConstBlockVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Const(expr_const) = expr {
            let inner_block: Expr = parse_quote! { ({ return (); }) };
            *expr = parse_quote! { const { #inner_block } };
            self.modified = true;
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}