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

pub struct Add_Recursive_Type_Alias_3;

impl Mutator for Add_Recursive_Type_Alias_3 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_name = &item_struct.ident;
                let alias_name = Ident::new(&format!("Recursive{}", struct_name), Span::call_site());
                let recursive_type: Type = parse_quote!(#alias_name);
                let type_alias: Item = parse_quote! {
                    type #alias_name = Option<Box<#recursive_type>>;
                };
                new_items.push(type_alias);
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias for each struct in the file. By creating a type alias that refers to itself via an `Option<Box<...>>`, it tests the compiler's handling of recursive types and type alias expansion, potentially leading to infinite recursion in type resolution or ICEs in the type checker."
    }
}