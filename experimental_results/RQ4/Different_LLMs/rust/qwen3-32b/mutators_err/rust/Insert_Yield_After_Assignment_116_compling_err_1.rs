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

pub struct Insert_Yield_After_Assignment_116;

impl Mutator for Insert_Yield_After_Assignment_116 {
    fn name(&self) -> &str {
        "Insert_Yield_After_Assignment_116"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Process top-level functions
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                self.process_block(&mut func.block);
            }
        }
        // Process closures in the AST
        struct ClosureVisitor;
        impl VisitMut for ClosureVisitor {
            fn visit_expr_closure_mut(&mut self, expr: &mut ExprClosure) {
                if let ExprClosure { body: box Expr::Block(block, ..), .. } = expr {
                    self.process_block(block);
                }
                syn::visit_mut::visit_expr_closure_mut(self, expr);
            }
        }
        let mut visitor = ClosureVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts an additional `yield` statement after an existing `yield` in a generator, immediately following a variable assignment. This increases the complexity of the generator's state machine, potentially causing the compiler's drop-tracking logic to miscalculate drop ranges or state transitions, exposing bugs in the generator's state management."
    }
}

impl Insert_Yield_After_Assignment_116 {
    fn process_block(&self, block: &mut syn::Block) {
        let mut stmts = &mut block.stmts;
        let len = stmts.len();
        let mut i = 0;
        while i < len {
            let stmt = &stmts[i];
            if let Stmt::Expr(expr) = stmt {
                if let Expr::Yield(yield_expr) = &**expr {
                    // Found a yield
                    if i + 1 < len {
                        let next_stmt = &stmts[i + 1];
                        if let Stmt::Expr(next_expr) = next_stmt {
                            if let Expr::Assign(assign_expr) = &**next_expr {
                                // Next statement is an assignment
                                // Insert a new yield after this
                                let new_yield = parse_quote! { yield; };
                                let new_stmt = Stmt::Expr(Box::new(new_yield), None);
                                stmts.insert(i + 2, new_stmt);
                                len += 1;
                                i += 2; // Skip to after the inserted statement
                                continue;
                            }
                        }
                    }
                }
            }
            i += 1;
        }
    }
}