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

pub struct Remove_Const_Keyword_427;

impl Mutator for Remove_Const_Keyword_427 {
    fn name(&self) -> &str {
        "Remove_Const_Keyword_427"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(local) = local {
                            if let PatType { attrs: _, pat, ty, .. } = &local.decl {
                                if let Pat::Ident(ident) = pat {
                                    if let Some(const_token) = ident.attrs.iter().find(|attr| attr.path().is_ident("const")) {
                                        ident.attrs.retain(|attr| !attr.path().is_ident("const"));
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
        "The Remove_Const_Keyword_427 mutation operator targets constant declarations within functions and removes the 'const' keyword. This transformation tests the compiler's handling of constants and their usage within functions, potentially triggering compiler errors or warnings."
    }
}