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

pub struct Closure_To_Generator_27;

impl Mutator for Closure_To_Generator_27 {
    fn name(&self) -> &str {
        "Closure_To_Generator_27"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if let Some(syn::TypeParamBound::Trait(trait_bound)) = type_impl_trait.bounds.first() {
                            if trait_bound.path.is_ident("Fn") {
                                let mut new_block = func.block.clone();
                                for stmt in &mut new_block.stmts {
                                    if let Stmt::Expr(expr) = stmt {
                                        if let Expr::Closure(closure) = expr {
                                            let yield_expr: Expr = parse_quote! { yield 42; };
                                            let return_expr: Expr = parse_quote! { 0 };
                                            let mut new_stmts = vec![Stmt::Expr(yield_expr)];
                                            new_stmts.push(Stmt::Expr(return_expr));
                                            closure.body = Box::new(Expr::Block(syn::ExprBlock {
                                                attrs: Vec::new(),
                                                label: None,
                                                block: syn::Block {
                                                    brace_token: token::Brace::default(),
                                                    stmts: new_stmts,
                                                },
                                            }));
                                        }
                                    }
                                }
                                func.block = new_block;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures within functions that return `impl Fn`. It transforms these closures into generator-like closures by introducing the `yield` keyword. This change is intended to test the Rust compiler's handling of closures and generators, potentially exposing issues in state management and type specialization."
    }
}