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

pub struct Add_Recursive_Type_Alias_5;

impl Mutator for Add_Recursive_Type_Alias_5 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut additional_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let recursive_alias: syn::Item = parse_quote! {
                    type RecursiveAlias = fn(RecursiveAlias);
                };
                additional_items.push(recursive_alias);
            }
            if let syn::Item::Impl(impl_item) = item {
                let recursive_alias: syn::ImplItem = parse_quote! {
                    type RecursiveAlias = fn(RecursiveAlias);
                };
                impl_item.items.push(recursive_alias);
            }
        }
        file.items.extend(additional_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a recursive type alias `type RecursiveAlias = fn(RecursiveAlias);` to function and impl blocks. This introduces a self-referential type alias that can challenge the compiler's type resolution and recursion handling mechanisms, potentially triggering ICEs or stack overflow errors due to infinite recursion in type alias resolution."
    }
}