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

pub struct Replace_Inline_Assembly_Parameter_174;

impl Mutator for Replace_Inline_Assembly_Parameter_174 {
    fn name(&self) -> &str {
        "Replace_Inline_Assembly_Parameter_174"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for statement in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = statement {
                        if let Expr::Call(ExprCall { args, .. }) = **expr {
                            for arg in &mut *args {
                                if let Expr::Lit(lit) = **arg {
                                    // Replace the literal with a different constant value
                                    *arg = parse_quote!(123);
                                } else if let Expr::Path(path) = **arg {
                                    // Replace the path with a different path
                                    *arg = parse_quote!(std::mem::size_of::<i32>());
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the parameters of inline assembly instructions with different constant values or paths. This transformation aims to test the compiler's handling of inline assembly parameters and may lead to ICEs or unexpected behavior in the compiler's code generation."
    }
}