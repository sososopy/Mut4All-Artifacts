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

pub struct InlineConstGenericTypeMethodCall_428;

impl Mutator for InlineConstGenericTypeMethodCall_428 {
    fn name(&self) -> &str {
        "InlineConstGenericTypeMethodCall_428"
    }
    fn mutate(&self, file: &mut syn::File) {
        let possible_methods = ["size_of", "needs_drop", "align_of", "min_align_of", "type_id", "variant_count"];
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(expr) = stmt {
                            if let Expr::Const(const_expr) = &mut expr.expr {
                                if let Expr::Path(path_expr) = &mut *const_expr.expr {
                                    let path = &mut path_expr.path;
                                    if path.segments.len() >= 3 
                                        && path.segments[0].ident == "std" 
                                        && path.segments[1].ident == "mem" 
                                    {
                                        let last_segment = path.segments.last_mut().unwrap();
                                        let current_method = last_segment.ident.to_string();
                                        if possible_methods.contains(&current_method.as_str()) {
                                            let mut rng = thread_rng();
                                            let candidates: Vec<&str> = possible_methods.iter().filter(|&&m| m != current_method).map(|&s| s).collect();
                                            if let Some(candidate) = candidates.choose(&mut rng) {
                                                last_segment.ident = Ident::new(candidate, last_segment.ident.span());
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline const blocks within generic const functions that invoke type-dependent intrinsics like std::mem::size_of::<T>(). It replaces these method calls with other type-based intrinsics (e.g., needs_drop, align_of), forcing the MIR builder to resolve different type metadata. This transformation stresses the compiler's handling of generic type metadata resolution, potentially exposing missing entries in type dependency maps."
    }
}