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

pub struct Inline_Const_Function_Mutation_41;

impl Mutator for Inline_Const_Function_Mutation_41 {
    fn name(&self) -> &str {
        "Inline_Const_Function_Mutation_41"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(expr) = &local.init {
                            if let Expr::Block(expr_block) = &*expr.expr {
                                if expr_block.attrs.iter().any(|attr| attr.path().is_ident("const")) {
                                    for stmt in &mut expr_block.block.stmts {
                                        if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                                            if let Expr::Path(expr_path) = &*expr_call.func {
                                                if expr_path.path.segments.iter().any(|seg| seg.ident == "transmute") {
                                                    let new_expr: Expr = parse_quote! {
                                                        core::mem::size_of::<u8>()
                                                    };
                                                    *stmt = Stmt::Expr(new_expr, None);
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