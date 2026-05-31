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

pub struct Alter_Slice_Pattern_In_Const_Fn_527;

impl Mutator for Alter_Slice_Pattern_In_Const_Fn_527 {
    fn name(&self) -> &str {
        "Alter_Slice_Pattern_In_Const_Fn_527"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Local(Local { pat, init, .. }) = stmt {
                            if let Some((_, expr)) = init {
                                if let Expr::Array(array) = &**expr {
                                    if let Pat::Slice(pat_slice) = pat {
                                        if pat_slice.elems.len() == 1 {
                                            if let Pat::Ident(pat_ident) = &pat_slice.elems[0] {
                                                if pat_ident.by_ref.is_none() && pat_ident.mutability.is_none() && pat_ident.subpat.is_some() {
                                                    let new_pat: Pat = parse_quote!([first, rest @ ..]);
                                                    *pat = new_pat;
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}