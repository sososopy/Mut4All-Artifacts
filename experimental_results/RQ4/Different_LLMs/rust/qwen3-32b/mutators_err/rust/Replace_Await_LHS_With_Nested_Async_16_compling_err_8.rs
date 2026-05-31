use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprAssign, ExprAwait, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Await_LHS_With_Nested_Async_16;

impl Mutator for Replace_Await_LHS_With_Nested_Async_16 {
    fn name(&self) -> &str {
        "Replace_Await_LHS_With_Nested_Async_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AssignmentVisitor;
        impl VisitMut for AssignmentVisitor {
            fn visit_expr_assign_mut(&mut self, e: &mut syn::ExprAssign) {
                // Check if left is a deref of an await
                if let syn::Expr::Unary(syn::ExprUnary {
                    expr: box_expr,
                    op: syn::UnOp::Deref(_),
                    ..
                }) = &*e.left
                {
                    if let syn::Expr::Await(await_expr) = &**box_expr {
                        // Create &mut await_expr
                        let mut_expr = Box::new(Expr::Reference(syn::ExprReference {
                            attrs: Vec::new(),
                            and_token: syn::token::And::default(),
                            mutability: Some(syn::token::Mut::default()),
                            expr: Box::new(Expr::Await(await_expr.clone())),
                        }));
                        // Create async block with mut_expr
                        let async_block = Box::new(Expr::Async(syn::ExprAsync {
                            attrs: Vec::new(),
                            async_token: syn::token::Async::default(),
                            capture: None,
                            block: Box::new(Expr::Block(syn::ExprBlock {
                                attrs: Vec::new(),
                                label: None,
                                block: syn::Block {
                                    brace_token: syn::token::Brace::default(),
                                    stmts: vec![Stmt::Expr(*mut_expr, None)],
                                },
                            })),
                        }));
                        // Create await of async_block
                        let await_expr_new = Box::new(Expr::Await(syn::ExprAwait {
                            attrs: Vec::new(),
                            base: async_block,
                            dot_token: None,
                        }));
                        // Create deref of await_expr_new
                        let deref_expr_new = Box::new(Expr::Unary(syn::ExprUnary {
                            attrs: Vec::new(),
                            op: {
                                let expr_unary = parse_quote!(*());
                                match expr_unary.op {
                                    syn::UnOp::Deref(token) => token,
                                    _ => unreachable!(),
                                }
                            },
                            expr: Box::new(*await_expr_new),
                        }));
                        // Replace left with deref_expr_new
                        *e.left = *deref_expr_new;
                    }
                }
                // Check if left is an await directly
                else if let syn::Expr::Await(await_expr) = &*e.left {
                    // Create &mut await_expr
                    let mut_expr = Box::new(Expr::Reference(syn::ExprReference {
                        attrs: Vec::new(),
                        and_token: syn::token::And::default(),
                        mutability: Some(syn::token::Mut::default()),
                        expr: Box::new(Expr::Await(await_expr.clone())),
                    }));
                    // Create async block with mut_expr
                    let async_block = Box::new(Expr::Async(syn::ExprAsync {
                        attrs: Vec::new(),
                        async_token: syn::token::Async::default(),
                        capture: None,
                        block: Box::new(Expr::Block(syn::ExprBlock {
                            attrs: Vec::new(),
                            label: None,
                            block: syn::Block {
                                brace_token: syn::token::Brace::default(),
                                stmts: vec![Stmt::Expr(*mut_expr, None)],
                            },
                        })),
                    }));
                    // Create await of async_block
                    let await_expr_new = Box::new(Expr::Await(syn::ExprAwait {
                        attrs: Vec::new(),
                        base: async_block,
                        dot_token: None,
                    }));
                    // Create deref of await_expr_new
                    let deref_expr_new = Box::new(Expr::Unary(syn::ExprUnary {
                        attrs: Vec::new(),
                        op: {
                            let expr_unary = parse_quote!(*());
                            match expr_unary.op {
                                syn::UnOp::Deref(token) => token,
                                _ => unreachable!(),
                            }
                        },
                        expr: Box::new(*await_expr_new),
                    }));
                    // Replace left with deref_expr_new
                    *e.left = *deref_expr_new;
                }
                // Continue visiting
                syn::visit_mut::visit_expr_assign_mut(self, e);
            }
        }
        let mut visitor = AssignmentVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces assignment left-hand sides that are await expressions with a nested async block that returns a mutable reference, then awaits and dereferences it. This transformation introduces multiple async scopes and nested mutable references, forcing the compiler to track drop order and manage complex borrowing patterns. It stresses the integration between async/await and Rust's borrow checker, particularly when intermediate await results are nested and require borrowing."
    }
}