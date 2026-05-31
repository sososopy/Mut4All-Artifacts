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

pub struct Introduce_Async_Enum_Destructure_Closure_Capture_46;

impl Mutator for Introduce_Async_Enum_Destructure_Closure_Capture_46 {
    fn name(&self) -> &str {
        "Introduce_Async_Enum_Destructure_Closure_Capture_46"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AsyncDestructureVisitor {
            mutations: Vec::new(),
        };
        visitor.visit_file_mut(file);
        for mutation in visitor.mutations {
            let async_block = mutation.async_block;
            let destructure_stmt = mutation.destructure_stmt;
            let destructure_pat = mutation.destructure_pat;
            let destructure_expr = mutation.destructure_expr;
            let span = destructure_stmt.span();
            let closure_stmt: Stmt = parse_quote! {
                let closure = move || {
                    let _ = #destructure_pat;
                };
                closure();
            };
            let mut new_stmts = Vec::new();
            for stmt in &async_block.block.stmts {
                if stmt.span().into() == span.into() {
                    new_stmts.push(destructure_stmt.clone());
                    new_stmts.push(closure_stmt);
                } else {
                    new_stmts.push(stmt.clone());
                }
            }
            async_block.block.stmts = new_stmts;
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async blocks containing enum destructuring patterns. It introduces a closure that captures the destructured binding, altering ownership analysis within the async state machine. This stresses the compiler's handling of captured destructured data in async contexts, potentially revealing bugs in closure capture, pattern destructuring, and async code generation interactions."
    }
}

struct AsyncDestructureMutation {
    async_block: syn::ExprAsync,
    destructure_stmt: syn::Stmt,
    destructure_pat: syn::Pat,
    destructure_expr: syn::Expr,
}

struct AsyncDestructureVisitor {
    mutations: Vec<AsyncDestructureMutation>,
}

impl VisitMut for AsyncDestructureVisitor {
    fn visit_expr_async_mut(&mut self, node: &mut syn::ExprAsync) {
        for stmt in &node.block.stmts {
            if let Stmt::Local(local) = stmt {
                if let Some(init) = &local.init {
                    if let Pat::Struct(pat_struct) = &local.pat {
                        if pat_struct.path.segments.last().unwrap().ident.to_string().contains("::") {
                            self.mutations.push(AsyncDestructureMutation {
                                async_block: node.clone(),
                                destructure_stmt: stmt.clone(),
                                destructure_pat: local.pat.clone(),
                                destructure_expr: *init.expr.clone(),
                            });
                        }
                    } else if let Pat::TupleStruct(pat_tuple_struct) = &local.pat {
                        if pat_tuple_struct.path.segments.last().unwrap().ident.to_string().contains("::") {
                            self.mutations.push(AsyncDestructureMutation {
                                async_block: node.clone(),
                                destructure_stmt: stmt.clone(),
                                destructure_pat: local.pat.clone(),
                                destructure_expr: *init.expr.clone(),
                            });
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_async_mut(self, node);
    }
}