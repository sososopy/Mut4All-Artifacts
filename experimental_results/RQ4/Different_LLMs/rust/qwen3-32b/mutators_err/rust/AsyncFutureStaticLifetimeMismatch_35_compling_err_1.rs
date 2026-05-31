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

pub struct AsyncFutureStaticLifetimeMismatch_35;

impl Mutator for AsyncFutureStaticLifetimeMismatch_35 {
    fn name(&self) -> &str {
        "AsyncFutureStaticLifetimeMismatch_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    if let ReturnType::Type(_, return_type) = &func.sig.output {
                        if let Type::ImplTrait(impl_trait) = &**return_type {
                            let new_local: Local = parse_quote! { let x = 0; };
                            let new_expr: Expr = parse_quote! { &x };
                            let mut new_block = func.block.clone();
                            new_block.stmts.clear();
                            new_block.stmts.push(Stmt::Local(new_local));
                            new_block.stmts.push(Stmt::Expr(new_expr, None));
                            *func.block = new_block;
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