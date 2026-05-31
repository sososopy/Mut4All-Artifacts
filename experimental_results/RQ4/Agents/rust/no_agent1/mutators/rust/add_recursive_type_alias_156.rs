use proc_macro2::{Span, TokenStream};
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

pub struct Add_Recursive_Type_Alias_156;

impl Mutator for Add_Recursive_Type_Alias_156 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_156"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_ident = &item_struct.ident;
                let type_alias_ident = Ident::new(&format!("Recursive{}", struct_ident), Span::call_site());

                let recursive_type: TokenStream = quote! {
                    type #type_alias_ident = Box<#type_alias_ident>;
                };

                new_items.push(Item::Verbatim(recursive_type));
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias for each struct in the file. This transformation creates a type alias that refers to itself through a `Box`, leading to potential infinite recursion in type resolution. It aims to test the compiler's ability to handle recursive types and may trigger ICEs or stack overflows during compilation."
    }
}