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

pub struct Replace_Closure_Block_With_Unsafe_106;

impl Mutator for Replace_Closure_Block_With_Unsafe_106 {
    fn name(&self) -> &str {
        "Replace_Closure_Block_With_Unsafe_106"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureMutatorVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closure expressions within blocks that are arguments to function calls. It mutates a closure body that is a block expression by replacing the block with just the `unsafe` keyword, creating a malformed closure. This transformation aims to trigger state inconsistencies in the compiler's THIR building phase, similar to the bug-inducing pattern. It applies when a closure is defined within a block used as a function argument, changing from `|parameters| { unsafe { statements } }` to `|parameters| unsafe`. This removes braces and inner statements, leaving only the `unsafe` keyword as the closure body."
    }
}

struct ClosureMutatorVisitor;

impl VisitMut for ClosureMutatorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(call_expr) = expr {
            for arg in &mut call_expr.args {
                self.visit_expr_mut(arg);
            }
        }
        if let Expr::Block(block_expr) = expr {
            for stmt in &mut block_expr.block.stmts {
                self.visit_stmt_mut(stmt);
            }
        }
        if let Expr::Closure(closure_expr) = expr {
            if let Expr::Block(block_expr) = &*closure_expr.body {
                if let Some(inner_block) = block_expr.block.stmts.iter().find_map(|stmt| {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Block(block) = expr {
                            Some(block)
                        } else {
                            None
                        }
                    } else {
                        None
                    }
                }) {
                    if inner_block.unsafe_token.is_some() {
                        closure_expr.body = parse_quote! { unsafe };
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}