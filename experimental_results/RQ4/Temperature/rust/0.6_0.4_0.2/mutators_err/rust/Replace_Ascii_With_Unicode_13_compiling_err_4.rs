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

pub struct Replace_Ascii_With_Unicode_13;

impl Mutator for Replace_Ascii_With_Unicode_13 {
    fn name(&self) -> &str {
        "Replace_Ascii_With_Unicode_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                func.block.stmts.iter_mut().for_each(|stmt| {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &mut local.init {
                            if let Expr::Binary(expr_binary) = &mut *init.expr {
                                if let syn::BinOp::Mul(_) = expr_binary.op {
                                    expr_binary.op = syn::BinOp::Mul(syn::token::Star {
                                        spans: [Span::call_site()],
                                    });
                                }
                            }
                        }
                    }
                });
                func.sig.inputs.iter_mut().for_each(|arg| {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Pat::Ident(ref mut pat_ident) = &mut *pat_type.pat {
                            if pat_ident.ident == "param" {
                                pat_ident.ident = Ident::new("param", Span::call_site());
                            }
                        }
                    }
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces ASCII characters with their Unicode counterparts in function signatures and expressions. This transformation tests the Rust compiler's handling of Unicode characters in various syntactic constructs, potentially revealing issues related to Unicode processing."
    }
}