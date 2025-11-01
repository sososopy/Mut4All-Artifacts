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

pub struct Async_Move_Clone_5;

impl Mutator for Async_Move_Clone_5 {
    fn name(&self) -> &str {
        "Async_Move_Clone_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AsyncClosureVisitor;
        
        impl VisitMut for AsyncClosureVisitor {
            fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
                if i.asyncness.is_some() && i.capture.is_some() {
                    let mut visitor = AwaitVariableVisitor;
                    visitor.visit_expr_mut(&mut i.body);
                }
                visit_mut::visit_expr_closure_mut(self, i);
            }
        }
        
        struct AwaitVariableVisitor;
        
        impl VisitMut for AwaitVariableVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Await(expr_await) = expr {
                    if let Expr::MethodCall(method_call) = &mut *expr_await.base {
                        if let Expr::Path(path) = &*method_call.receiver {
                            if let Some(ident) = path.path.get_ident() {
                                let clone_ident = Ident::new(&format!("{}_clone", ident), Span::call_site());
                                let clone_stmt: Stmt = parse_quote! {
                                    let #clone_ident = #ident.clone();
                                };
                                let new_receiver: Expr = parse_quote! {
                                    #clone_ident
                                };
                                method_call.receiver = Box::new(new_receiver);
                                expr_await.base = Box::new(Expr::MethodCall(method_call.clone()));
                                let closure_body = expr_await.span().unwrap();
                                let mut stmts = vec![clone_stmt];
                                stmts.append(&mut vec![Stmt::Expr(expr.clone())]);
                                *expr = Expr::Block(ExprBlock {
                                    attrs: vec![],
                                    label: None,
                                    block: Block {
                                        brace_token: token::Brace { span: closure_body },
                                        stmts,
                                    },
                                });
                            }
                        }
                    }
                }
                visit_mut::visit_expr_mut(self, expr);
            }
        }
        
        let mut visitor = AsyncClosureVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies async move closures within chained futures operations. It locates variables moved with the 'move' keyword used in 'await' operations. For these variables, it introduces a clone before the await operation, replacing the original variable's usage with the clone. This mutation targets potential compiler issues related to borrow checking or internal state management by adding unnecessary data copies."
    }
}