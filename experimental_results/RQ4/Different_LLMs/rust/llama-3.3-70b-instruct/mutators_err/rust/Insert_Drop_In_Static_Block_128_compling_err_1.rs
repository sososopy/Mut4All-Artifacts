use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Insert_Drop_In_Static_Block_128;

impl Mutator for Insert_Drop_In_Static_Block_128 {
    fn name(&self) -> &str {
        "Insert_Drop_In_Static_Block_128"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                let mut new_stmts = Vec::new();
                for stmt in &static_item.block.stmts {
                    new_stmts.push(stmt.clone());
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Array(array) = &**init {
                                let drop_stmt = Stmt::Expr(
                                    parse_quote! {
                                        drop(#local.pat);
                                    },
                                    None,
                                );
                                new_stmts.push(drop_stmt);
                            }
                        }
                    }
                }
                static_item.block.stmts = new_stmts;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a drop statement for variables in static blocks, especially when the variable is an array or collection. This transformation aims to test the compiler's handling of drop order and const evaluation, potentially exposing bugs related to destructor calls and static initialization."
    }
}