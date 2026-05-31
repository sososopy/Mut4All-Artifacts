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

pub struct Replace_Asm_With_Function_Call_173;

impl Mutator for Replace_Asm_With_Function_Call_173 {
    fn name(&self) -> &str {
        "Replace_Asm_With_Function_Call_173"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Macro(mac) = expr {
                            if mac.mac.path.is_ident("asm") {
                                let args = mac.mac.tokens.clone();
                                let mut irq = None;
                                let mut shim = None;
                                for arg in args.into_iter() {
                                    if let TokenTree::Group(group) = arg {
                                        let group = group.stream();
                                        for token in group.into_iter() {
                                            if let TokenTree::Ident(ident) = token {
                                                if ident == "irq" {
                                                    irq = Some(ident.clone());
                                                } else if ident == "shim" {
                                                    shim = Some(ident.clone());
                                                }
                                            }
                                        }
                                    }
                                }
                                if let (Some(irq), Some(shim)) = (irq, shim) {
                                    let simulate_asm = quote! {
                                        simulate_asm(#irq, #shim)
                                    };
                                    *stmt = Stmt::Expr(Expr::Verbatim(simulate_asm), Default::default());
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces instances of inline assembly code defined by the asm! macro with a function call to simulate the behavior of the assembly code. This transformation aims to test the Rust compiler's ability to handle the interaction between Rust code and assembly code, and potentially expose bugs related to this interaction."
    }
}