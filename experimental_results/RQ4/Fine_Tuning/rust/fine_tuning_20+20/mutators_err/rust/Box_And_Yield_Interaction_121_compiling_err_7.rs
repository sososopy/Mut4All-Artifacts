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

pub struct Box_And_Yield_Interaction_121;

impl Mutator for Box_And_Yield_Interaction_121 {
    fn name(&self) -> &str {
        "Box_And_Yield_Interaction_121"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    let mut new_stmts = vec![];
                    for stmt in &func.block.stmts {
                        if let Stmt::Expr(Expr::Closure(closure), None) = stmt {
                            if let Expr::Box(boxed) = &*closure.body {
                                if let Expr::Yield(_) = &*boxed.expr {
                                    new_stmts.push(parse_quote! {
                                        let mut gen_box = Box::new(|| yield);
                                    });
                                    continue;
                                }
                            }
                        }
                        new_stmts.push(stmt.clone());
                    }
                    func.block.stmts = new_stmts;
                }
            }
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let mut new_stmts = vec![];
                    for stmt in &func.block.stmts {
                        if let Stmt::Expr(Expr::Closure(closure), None) = stmt {
                            if let Expr::Box(boxed) = &*closure.body {
                                if let Expr::Yield(_) = &*boxed.expr {
                                    new_stmts.push(parse_quote! {
                                        let mut gen_box = Box::new(|| yield);
                                    });
                                    continue;
                                }
                            }
                        }
                        new_stmts.push(stmt.clone());
                    }
                    func.block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets closures that contain a boxed generator expression using yield. It transforms these expressions into a boxed assignment to a mutable variable, leveraging Box::new to encapsulate the generator. This mutation stresses the compiler's handling of boxed generator expressions and mutable references, potentially exposing issues related to memory management, borrowing, and control flow in generator contexts."
    }
}