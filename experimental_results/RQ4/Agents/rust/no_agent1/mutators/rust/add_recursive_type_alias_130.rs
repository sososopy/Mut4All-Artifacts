use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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
};

use crate::mutator::Mutator;

pub struct Add_Recursive_Type_Alias_130;

impl Mutator for Add_Recursive_Type_Alias_130 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_130"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_ident = &item_struct.ident;
                let recursive_type_alias: Item = parse_quote! {
                    type RecursiveAlias = Box<dyn #struct_ident>;
                };
                new_items.push(recursive_type_alias);
            }
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let func_ident = &func.sig.ident;
                    let recursive_type_alias: Item = parse_quote! {
                        type RecursiveAlias = Box<dyn Fn() -> #func_ident>;
                    };
                    new_items.push(recursive_type_alias);
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias for each struct and non-main function. By creating a `Box<dyn Struct>` or `Box<dyn Fn() -> Function>` alias, it aims to test the compiler's handling of recursive type definitions and dynamic dispatch, potentially leading to ICEs or type resolution issues."
    }
}