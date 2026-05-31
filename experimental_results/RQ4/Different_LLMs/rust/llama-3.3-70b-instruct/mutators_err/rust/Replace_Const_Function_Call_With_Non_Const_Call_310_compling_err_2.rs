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

pub struct Replace_Const_Function_Call_With_Non_Const_Call_310;

impl Mutator for Replace_Const_Function_Call_With_Non_Const_Call_310 {
    fn name(&self) -> &str {
        "Replace_Const_Function_Call_With_Non_Const_Call_310"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call) = expr {
                            if let Expr::Path(path) = &*call.func {
                                if let Some(const_token) = path.path.segments.first() {
                                    if let Some(arguments) = &const_token.arguments {
                                        if let PathArguments::AngleBracketed(angle_bracketed) = arguments {
                                            if angle_bracketed.args.is_empty() {
                                                // Remove const keyword
                                                const_token.arguments = None;
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
        "The mutation operator replaces const function calls with non-const function calls. It targets function calls within the code and removes the const keyword if present, changing the function call from const to non-const. This transformation aims to test the compiler's handling of const functions and closures, potentially triggering bugs in the const evaluation system."
    }
}