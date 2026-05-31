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

pub struct Intrinsic_Function_Mutation_41;

impl Mutator for Intrinsic_Function_Mutation_41 {
    fn name(&self) -> &str {
        "Intrinsic_Function_Mutation_41"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Block(expr_block), _) = stmt {
                        if let Some(first_stmt) = expr_block.block.stmts.first_mut() {
                            if let Stmt::Expr(Expr::Path(expr_path), _) = first_stmt {
                                let path = &mut expr_path.path;
                                if path.segments.len() == 3 && path.segments[0].ident == "core" && path.segments[1].ident == "mem" {
                                    let intrinsic_name = &path.segments[2].ident;
                                    if intrinsic_name == "transmute" {
                                        path.segments[2].ident = Ident::new("swap", Span::call_site());
                                        if let PathArguments::AngleBracketed(args) = &mut path.segments[2].arguments {
                                            args.args = Punctuated::from_iter(vec![GenericArgument::Type(parse_quote!(u8))]);
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
        "The mutation operator targets intrinsic functions within inline constant blocks, specifically changing the `transmute` function to `swap` and altering type parameters. This transformation aims to expose potential bugs in the compiler's handling of intrinsic operations by modifying sensitive intrinsic function calls."
    }
}