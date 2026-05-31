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

pub struct Add_Nested_Type_Alias_56;

impl Mutator for Add_Nested_Type_Alias_56 {
    fn name(&self) -> &str {
        "Add_Nested_Type_Alias_56"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut rng = thread_rng();
                let nested_alias_count = rng.gen_range(1..=5);

                let mut nested_aliases = Vec::new();
                for i in 0..nested_alias_count {
                    let alias_ident = Ident::new(&format!("NestedAlias{}", i), Span::call_site());
                    let alias_type: Type = parse_quote!(usize);
                    let type_alias: Item = parse_quote!(type #alias_ident = #alias_type;);
                    nested_aliases.push(type_alias);
                }

                let struct_generics = &mut item_struct.generics;
                struct_generics.params.push(parse_quote!(T));
                struct_generics.params.push(parse_quote!(U));

                for alias in nested_aliases {
                    file.items.push(alias);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds multiple nested type aliases within struct definitions and expands the generics of the struct with new types. This transformation aims to stress the compiler's type resolution system by introducing complex type aliasing scenarios, potentially leading to ICEs or resolution failures when deeply nested types are involved."
    }
}