use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Recursive_Type_Alias_99;

impl Mutator for Add_Recursive_Type_Alias_99 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_99"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_name = &item_struct.ident;
                let recursive_alias: Item = parse_quote! {
                    type RecursiveAlias = Box<#struct_name>;
                };
                new_items.push(recursive_alias);
            }
            if let syn::Item::Enum(item_enum) = item {
                let enum_name = &item_enum.ident;
                let recursive_alias: Item = parse_quote! {
                    type RecursiveAlias = Box<#enum_name>;
                };
                new_items.push(recursive_alias);
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias for structs and enums, encapsulating them in a Box. This transformation creates potentially infinite type recursion, aiming to stress the compiler's type resolution and memory management systems. It tests the compiler's ability to handle deeply nested types and may trigger ICEs related to type alias resolution or stack overflows."
    }
}