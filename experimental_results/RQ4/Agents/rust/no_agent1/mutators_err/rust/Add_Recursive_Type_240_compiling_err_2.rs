use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemStructFields, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Recursive_Type_240;

impl Mutator for Add_Recursive_Type_240 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_240"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_name = &item_struct.ident;
                let recursive_field: syn::Field = parse_quote! {
                    recursive: Option<Box<#struct_name>>
                };
                if let syn::Fields::Named(ref mut fields) = item_struct.fields {
                    fields.named.push(recursive_field);
                }
            }
            if let syn::Item::Enum(item_enum) = item {
                let enum_name = &item_enum.ident;
                let recursive_variant: syn::Variant = parse_quote! {
                    Recursive(Box<#enum_name>)
                };
                item_enum.variants.push(recursive_variant);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a recursive type definition to structs and enums. For structs, it adds a field that is an `Option<Box<Self>>`, and for enums, it adds a variant that contains a `Box<Self>`. This transformation creates deeply nested types that can challenge the compiler's handling of recursive type definitions, potentially leading to stack overflows or infinite loops during type checking or trait resolution."
    }
}