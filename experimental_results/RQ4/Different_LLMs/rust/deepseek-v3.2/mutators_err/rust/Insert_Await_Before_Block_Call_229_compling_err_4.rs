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

pub struct Insert_Await_Before_Block_Call_229;

impl Mutator for Insert_Await_Before_Block_Call_229 {
    fn name(&self) -> &str {
        "Insert_Await_Before_Block_Call_229"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AwaitInserter::default();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts an `await` keyword before block expressions that are used as function call arguments or directly as call receivers. It targets expressions like `{...}()` and `foo({...})`, transforming them to `await {...}()` and `foo(await {...})`. This replicates patterns that trigger internal compiler errors due to incorrect span handling in error reporting. The mutation applies only to Rust 2018+ editions where `await` is a keyword, aiming to expose parser and diagnostics bugs related to `.await` syntax interaction with invalid expression contexts."
    }
}

#[derive(Default)]
struct AwaitInserter;

impl VisitMut for AwaitInserter {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        match expr {
            Expr::Call(call_expr) => {
                if let Expr::Block(block_expr) = &mut *call_expr.func {
                    let new_expr: Expr = parse_quote! { await #block_expr };
                    call_expr.func = Box::new(new_expr);
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
            Expr::MethodCall(method_call) => {
                if let Expr::Block(block_expr) = &mut *method_call.receiver {
                    let new_expr: Expr = parse_quote! { await #block_expr };
                    method_call.receiver = Box::new(new_expr);
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
            _ => {
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
    }
    
    fn visit_expr_call_mut(&mut self, call_expr: &mut ExprCall) {
        for arg in &mut call_expr.args {
            if let Expr::Block(block_expr) = arg {
                let new_expr: Expr = parse_2quote! { await #block_expr };
                *arg = new_expr;
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, call_expr);
    }
    
    fn visit_expr_method_call_mut(&mut self, method_call: &mut ExprMethodCall) {
        for arg in &mut method_call.args {
            if let Expr::Block(block_expr) = arg {
                let new_expr: Expr = parse_quote! { await #block_expr };
                *arg = new_expr;
            }
        }
        syn::visit_mut::visit_expr_method_call_mut(self, method_call);
    }
}