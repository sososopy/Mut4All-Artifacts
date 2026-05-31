use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Modify_Closure_Capture_19;

impl Mutator for Modify_Closure_Capture_19 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_19"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut rng = thread_rng();
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Closure(closure) = &*init.expr {
                                let mut new_stmts = closure.body.clone();
                                if let Expr::Block(block) = &mut *new_stmts {
                                    let new_var: Stmt = parse_quote! {
                                        let another_var = 20;
                                    };
                                    block.block.stmts.insert(0, parse_quote! {
                                        let _ = &another_var;
                                    });
                                    block.block.stmts.insert(0, new_var);
                                }
                                *stmt = parse_quote! {
                                    let _closure = #new_stmts;
                                };
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies closures by changing variable capture methods. It introduces a new variable and captures it by reference within the closure. This transformation aims to test the compiler's handling of lifetime inference and borrowing rules, potentially exposing issues in lifetime analysis and type inference."
    }
}