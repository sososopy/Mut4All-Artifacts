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

pub struct Add_Recursive_Type_Alias_297;

impl Mutator for Add_Recursive_Type_Alias_297 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_297"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_ident = &item_struct.ident;
                let recursive_alias: syn::Item = parse_quote! {
                    type RecursiveAlias = Box<#struct_ident>;
                };
                file.items.push(recursive_alias);
            }
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let recursive_alias: syn::Item = parse_quote! {
                        type RecursiveAlias = fn(RecursiveAlias) -> RecursiveAlias;
                    };
                    file.items.push(recursive_alias);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a recursive type alias to both structs and non-main functions. This transformation introduces potentially infinite type recursion, which can lead to compiler stack overflows, ICEs, or hangs during type resolution and checking. It aims to challenge the compiler's handling of recursive types and aliasing mechanisms."
    }
}