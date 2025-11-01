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

pub struct Replace_Closure_With_Coroutine_In_Const_Context_174;

impl Mutator for Replace_Closure_With_Coroutine_In_Const_Context_174 {
    fn name(&self) -> &str {
        "Replace_Closure_With_Coroutine_In_Const_Context_174"
    }

    fn mutate(&self, file: &mut syn::File) {
        struct ClosureToCoroutineReplacer;

        impl VisitMut for ClosureToCoroutineReplacer {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Closure(closure) = expr {
                    *expr = Expr::Async(syn::ExprAsync {
                        attrs: vec![],
                        async_token: token::Async { span: closure.span() },
                        block: syn::Block {
                            brace_token: token::Brace { span: closure.body.span().unwrap() },
                            stmts: if let Expr::Block(block) = &*closure.body {
                                block.block.stmts.clone()
                            } else {
                                vec![syn::Stmt::Expr(*closure.body.clone(), None)]
                            },
                        },
                        capture: None,
                    });
                } else {
                    syn::visit_mut::visit_expr_mut(self, expr);
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.generics.params.iter().any(|param| matches!(param, GenericParam::Const(_))) {
                    ClosureToCoroutineReplacer.visit_block_mut(&mut *item_fn.block);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions using generic const expressions with closures in constant contexts. It replaces closures with coroutines using async/await, testing the compiler's handling of advanced features in constant contexts."
    }
}