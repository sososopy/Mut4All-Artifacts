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

pub struct Inline_Assembly_With_Invalid_Type_170;

impl Mutator for Inline_Assembly_With_Invalid_Type_170 {
    fn name(&self) -> &str {
        "Inline_Assembly_With_Invalid_Type_170"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let syn::Fields::Named(ref mut named) = item_struct.fields {
                    if let Some(field) = named.named.first_mut() {
                        field.ty = parse_quote!(InvalidType);
                    }
                } else if let syn::Fields::Unnamed(ref mut unnamed) = item_struct.fields {
                    if let Some(field) = unnamed.unnamed.first_mut() {
                        field.ty = parse_quote!(InvalidType);
                    }
                }
            }

            if let syn::Item::Fn(item_fn) = item {
                if item_fn.sig.unsafety.is_some() {
                    let param_name = item_fn.sig.inputs.iter().find_map(|arg| {
                        if let FnArg::Typed(pat_type) = arg {
                            if let Pat::Ident(ident) = &*pat_type.pat {
                                Some(ident.ident.clone())
                            } else {
                                None
                            }
                        } else {
                            None
                        }
                    });

                    if let Some(name) = param_name {
                        let let_code = format!("let p = {} as *const _;", name);
                        let let_stmt: Stmt = syn::parse_str(&let_code).unwrap();

                        let asm_code = "core::arch::asm!(\"nop\", in(\"rax\") p,);";
                        let asm_expr: Expr = syn::parse_str(asm_code).unwrap();
                        let asm_stmt = Stmt::Expr(asm_expr, None);

                        if let Some(block) = &mut item_fn.block {
                            block.stmts.push(let_stmt);
                            block.stmts.push(asm_stmt);
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