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

pub struct Modify_Closure_Capture_131;

impl Mutator for Modify_Closure_Capture_131 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_131"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Closure(closure)) = stmt {
                            let new_var: Stmt = parse_quote! {
                                let buffer: &[u8] = b"bar";
                            };
                            item_fn.block.stmts.insert(0, new_var);

                            if let Expr::Block(block) = &mut *closure.body {
                                if let Some(first_stmt) = block.stmts.first_mut() {
                                    if let Stmt::Local(local) = first_stmt {
                                        if let Some((_, expr)) = &mut local.init {
                                            *expr = parse_quote!(buffer);
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
        "Identify closures within const functions and introduce a variable outside the closure, then modify the closure to capture this variable by using it inside."
    }
}