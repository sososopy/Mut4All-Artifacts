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

pub struct Unsafe_Transmute_In_Function_Context_223;

impl Mutator for Unsafe_Transmute_In_Function_Context_223 {
    fn name(&self) -> &str {
        "Unsafe_Transmute_In_Function_Context_223"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Path(TypePath { path, .. }) = &**return_type {
                        if let Some(segment) = path.segments.last() {
                            if segment.ident == "u64" {
                                let mut option_var_ident = None;
                                for stmt in &func.block.stmts {
                                    if let Stmt::Local(Local { pat, init, .. }) = stmt {
                                        if let Some(init) = init {
                                            if let Expr::Path(ExprPath { path, .. }) = &*init.expr {
                                                if let Some(segment) = path.segments.last() {
                                                    if segment.ident == "Option" {
                                                        if let Pat::Ident(pat_ident) = pat {
                                                            option_var_ident = Some(pat_ident.ident.clone());
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                if let Some(option_var_ident) = option_var_ident {
                                    let transmute_fn: ItemFn = parse_quote! {
                                        unsafe fn transmute_option(option: Option<u32>) -> u64 {
                                            std::mem::transmute(option)
                                        }
                                    };

                                    func.block.stmts.insert(0, Stmt::Item(Item::Fn(transmute_fn)));

                                    for stmt in &mut func.block.stmts {
                                        if let Stmt::Local(Local { init, .. }) = stmt {
                                            if let Some(init) = init {
                                                if let Expr::Unsafe(expr_unsafe) = init.expr.as_mut() {
                                                    if let Some(Expr::Call(ExprCall { func, args, .. })) = expr_unsafe.block.stmts.get_mut(0).and_then(|stmt| {
                                                        if let Stmt::Expr(expr, _) = stmt {
                                                            Some(expr)
                                                        } else {
                                                            None
                                                        }
                                                    }) {
                                                        if let Expr::Path(ExprPath { path, .. }) = &**func {
                                                            if path.is_ident("std::mem::transmute") {
                                                                *func = Box::new(Expr::Path(ExprPath {
                                                                    attrs: vec![],
                                                                    qself: None,
                                                                    path: parse_quote!(transmute_option),
                                                                }));

                                                                args.clear();
                                                                args.push(parse_quote!(#option_var_ident));
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
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator identifies function definitions with a return type of `u64` and a variable of type `Option<T>`. It refactors the transmutation logic into a separate unsafe function `transmute_option`, which takes an `Option<T>` and returns a `u64`. This separation of concerns could affect optimization and expose latent bugs related to transmutation in the compiler's handling of unsafe code."
    }
}