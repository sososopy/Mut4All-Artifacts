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

pub struct Replace_Box_Allocator_5;

impl Mutator for Replace_Box_Allocator_5 {
    fn name(&self) -> &str {
        "Replace_Box_Allocator_5"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut allocators = vec![];
        for item in &file.items {
            if let syn::Item::Struct(struct_item) = item {
                if struct_item.fields.iter().any(|field| {
                    if let syn::Type::Path(type_path) = &field.ty {
                        type_path.path.segments.last().unwrap().ident == "Allocator"
                    } else {
                        false
                    }
                }) {
                    allocators.push(struct_item.ident.clone());
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(ExprCall { func, args, .. }) = expr {
                            if let Expr::Path(ExprPath { path, .. }) = **func {
                                if path.segments.last().unwrap().ident == "Box" {
                                    if let Some(arg) = args.get(1) {
                                        if let Expr::Call(ExprCall { func: Expr::Path(ExprPath { path: allocator_path, .. }), .. }) = arg {
                                            if let Some(replacement_allocator) = allocators.iter().find(|allocator| **allocator != allocator_path.segments.last().unwrap().ident) {
                                                args[1] = parse_quote!(&#replacement_allocator);
                                            } else {
                                                args.pop();
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
        "The mutation operator replaces custom allocators in Box::new_in expressions with other existing allocators in the program or the default global allocator. This transformation aims to test the compiler's handling of custom allocators and their interaction with optimizations and features like LTO."
    }
}