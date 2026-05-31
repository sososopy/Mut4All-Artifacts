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

pub struct Add_Recursive_Type_Alias_313;

impl Mutator for Add_Recursive_Type_Alias_313 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_313"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_ident = &item_struct.ident;
                let type_alias: Item = parse_quote! {
                    type RecursiveAlias = #struct_ident<RecursiveAlias>;
                };
                new_items.push(type_alias);
            }
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let type_alias: Item = parse_quote! {
                    type RecursiveAliasFn = fn(RecursiveAliasFn);
                };
                new_items.push(type_alias);
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces recursive type aliases within structs and functions. By creating self-referential type definitions, it aims to stress the compiler's type resolution and recursion handling mechanisms, potentially leading to ICEs or infinite loops during type checking and resolution."
    }
}