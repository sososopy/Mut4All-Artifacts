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

pub struct Replace_Closure_Capture_By_Reference_With_By_Value_43;

impl Mutator for Replace_Closure_Capture_By_Reference_With_By_Value_43 {
    fn name(&self) -> &str {
        "Replace_Closure_Capture_By_Reference_With_By_Value_43"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Closure(closure) = &**expr {
                            if closure.move_token.is_none() {
                                closure.move_token = Some(token::Move {
                                    span: Span::call_site(),
                                });
                            }
                            for cap in &mut closure.captures {
                                if let syn::Capture::ByRef(ref_mut) = cap {
                                    *cap = syn::Capture::ByValue(ref_mut);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces closure captures by reference with captures by value. It targets closures that capture variables from their environment and changes the capture mode from reference to value. This transformation aims to test the compiler's handling of closure captures and variable lifetimes, potentially exposing bugs related to capture modes and ownership."
    }
}