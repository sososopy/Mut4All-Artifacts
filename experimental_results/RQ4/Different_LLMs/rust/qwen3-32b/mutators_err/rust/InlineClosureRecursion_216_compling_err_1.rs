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

pub struct InlineClosureRecursion_216;

impl Mutator for InlineClosureRecursion_216 {
    fn name(&self) -> &str {
        "InlineClosureRecursion_216"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let stmts = &mut func.block.stmts;
                let mut i = 0;
                while i < stmts.len() {
                    if let Stmt::Local(ref mut local) = stmts[i] {
                        if let Some(init) = &mut local.init {
                            if let Expr::Closure(closure) = &mut *init.expr {
                                if closure.capture.is_none() {
                                    let data_stmt = parse_quote! { let data = 0u8; };
                                    stmts.insert(i, data_stmt);
                                    i += 1;
                                    
                                    if let Stmt::Local(ref mut local) = stmts[i] {
                                        if let Some(init) = &mut local.init {
                                            if let Expr::Closure(closure) = &mut *init.expr {
                                                if closure.capture.is_none() {
                                                    if let Pat::Ident(pat_ident) = &local.pat {
                                                        let func_name = &pat_ident.ident;
                                                        let original_body = closure.body.as_ref().clone();
                                                        let new_body = parse_quote! {
                                                            if data < 1 {
                                                                #func_name(data)
                                                            } else {
                                                                #original_body
                                                            }
                                                        };
                                                        closure.body = Box::new(ExprBlock {
                                                            attrs: vec![],
                                                            label: None,
                                                            block: parse_quote! { { #new_body } },
                                                        });
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    i += 1;
                                } else {
                                    i += 1;
                                }
                            } else {
                                i += 1;
                            }
                        } else {
                            i += 1;
                        }
                    } else {
                        i += 1;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms non-capturing closures into recursive closures that capture a variable from the outer scope. This creates a cyclic call graph with captured values, testing the compiler's ability to handle closure captures, recursion, and MIR inliner cycle detection. The mutation introduces a new variable in the outer scope, modifies the closure to capture it, and replaces the body with a recursive call conditionally based on the captured value."
    }
}