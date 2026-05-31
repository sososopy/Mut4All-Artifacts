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

pub struct Modify_Closure_Usage_500;

impl Mutator for Modify_Closure_Usage_500 {
    fn name(&self) -> &str {
        "Modify_Closure_Usage_500"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &mut local.init {
                            if let Expr::Closure(closure) = expr.as_mut() {
                                if let Some(BoundLifetimes { ref lifetimes, .. }) = closure.lifetimes {
                                    if !lifetimes.is_empty() {
                                        closure.lifetimes = None; // Remove lifetime annotations
                                        if let Some(inputs) = closure.inputs.iter_mut().next() {
                                            if let FnArg::Typed(PatType { ref mut ty, .. }) = inputs {
                                                if let Type::Reference(ref mut ref_type) = **ty {
                                                    ref_type.lifetime = None; // Remove lifetime from parameter type
                                                }
                                            }
                                        }
                                        if let Type::Reference(ref mut ref_type) = *closure.output {
                                            ref_type.lifetime = None; // Remove lifetime from return type
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closure expressions that use explicit lifetime annotations. It removes these annotations from the closure's parameters and return types, aiming to stress test the compiler's lifetime resolution and borrow checking mechanisms. This transformation can expose issues in how the Rust compiler handles lifetimes, potentially leading to unresolved lifetimes or incorrect borrow checks."
    }
}