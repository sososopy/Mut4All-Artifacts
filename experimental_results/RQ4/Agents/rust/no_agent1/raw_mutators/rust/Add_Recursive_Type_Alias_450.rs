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

pub struct Add_Recursive_Type_Alias_450;

impl Mutator for Add_Recursive_Type_Alias_450 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_450"
    }
    fn mutate(&self, file: &mut syn::File) {
        let recursive_alias: Item = parse_quote! {
            type RecursiveAlias = Vec<RecursiveAlias>;
        };

        file.items.push(recursive_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let new_stmt: Stmt = parse_quote! {
                    let _x: RecursiveAlias = Vec::new();
                };
                func.block.stmts.insert(0, new_stmt);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveAlias = Vec<RecursiveAlias>;` and uses it in non-main functions. This transformation aims to test the compiler's ability to handle recursive type definitions and may lead to stack overflow or infinite recursion issues during type checking or monomorphization."
    }
}