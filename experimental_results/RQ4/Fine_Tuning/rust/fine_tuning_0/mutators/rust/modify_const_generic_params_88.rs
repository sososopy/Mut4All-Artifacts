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

pub struct Modify_Const_Generic_Params_88;

impl Mutator for Modify_Const_Generic_Params_88 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Params_88"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics.params;
                let mut modified = false;
                for param in generics.iter_mut() {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let syn::Type::Path(type_path) = &const_param.ty {
                            if type_path.path.is_ident("&'static str") {
                                const_param.ty = parse_quote!(usize);
                                modified = true;
                            }
                        }
                    }
                }
                if modified {
                    let mut new_block = func.block.clone();
                    for stmt in &mut new_block.stmts {
                        match stmt {
                            Stmt::Local(local) => {
                                if let Some(init) = &mut local.init {
                                    if let Expr::Path(expr_path) = &mut *init.expr {
                                        if let Some(ident) = expr_path.path.get_ident() {
                                            if ident == "A" || ident == "B" {
                                                *init.expr = parse_quote!(0);
                                            }
                                        }
                                    }
                                }
                            }
                            Stmt::Expr(expr, _) => {
                                if let Expr::Path(expr_path) = expr {
                                    if let Some(ident) = expr_path.path.get_ident() {
                                        if ident == "A" || ident == "B" {
                                            *expr = parse_quote!(0);
                                        }
                                    }
                                }
                            }
                            _ => {}
                        }
                    }
                    func.block = new_block;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}