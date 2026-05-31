use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Modify_Trait_Impl_With_Assoc_Type_454;

impl Mutator for Modify_Trait_Impl_With_Assoc_Type_454 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_With_Assoc_Type_454"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut candidates = vec![];
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Type(_) = item {
                        candidates.push(item_trait.ident.to_string());
                    }
                }
            }
        }
        let mut choices = vec![];
        for item in &file.items {
            match item {
                Item::Struct(_) => {
                    choices.push("struct".to_string());
                }
                Item::Enum(_) => {
                    choices.push("enum".to_string());
                }
                Item::Trait(_) => {
                    choices.push("trait".to_string());
                }
                Item::Type(_) => {
                    choices.push("type".to_string());
                }
                Item::Fn(_) => {
                    choices.push("fn".to_string());
                }
                _ => {}
            }
        }
        let struct_names: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| {
                if let Item::Struct(item_struct) = item {
                    Some(item_struct.ident.to_string())
                } else {
                    None
                }
            })
            .collect();
        let enum_names: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| {
                if let Item::Enum(item_enum) = item {
                    Some(item_enum.ident.to_string())
                } else {
                    None
                }
            })
            .collect();
        let trait_names: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| {
                if let Item::Trait(item_trait) = item {
                    Some(item_trait.ident.to_string())
                } else {
                    None
                }
            })
            .collect();
        let type_names: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| {
                if let Item::Type(item_type) = item {
                    Some(item_type.ident.to_string())
                } else {
                    None
                }
            })
            .collect();
        let fn_names: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| {
                if let Item::Fn(item_fn) = item {
                    Some(item_fn.sig.ident.to_string())
                } else {
                    None
                }
            })
            .collect();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if candidates.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for item in &mut item_impl.items {
                            if let syn::ImplItem::Type(impl_type) = item {
                                let mut rng = thread_rng();
                                let choice = choices.choose(&mut rng).unwrap().clone();
                                let new_type = match choice.as_str() {
                                    "struct" => {
                                        if struct_names.is_empty() {
                                            impl_type.ty.clone()
                                        } else {
                                            let chosen_struct =
                                                struct_names.choose(&mut rng).unwrap();
                                            parse_quote!(#chosen_struct)
                                        }
                                    }
                                    "enum" => {
                                        if enum_names.is_empty() {
                                            impl_type.ty.clone()
                                        } else {
                                            let chosen_enum = enum_names.choose(&mut rng).unwrap();
                                            parse_quote!(#chosen_enum)
                                        }
                                    }
                                    "trait" => {
                                        if trait_names.is_empty() {
                                            impl_type.ty.clone()
                                        } else {
                                            let chosen_trait =
                                                trait_names.choose(&mut rng).unwrap();
                                            parse_quote!(dyn #chosen_trait)
                                        }
                                    }
                                    "type" => {
                                        if type_names.is_empty() {
                                            impl_type.ty.clone()
                                        } else {
                                            let chosen_type = type_names.choose(&mut rng).unwrap();
                                            parse_quote!(#chosen_type)
                                        }
                                    }
                                    "fn" => {
                                        if fn_names.is_empty() {
                                            impl_type.ty.clone()
                                        } else {
                                            let chosen_fn = fn_names.choose(&mut rng).unwrap();
                                            parse_quote!(fn #chosen_fn)
                                        }
                                    }
                                    _ => impl_type.ty.clone(),
                                };
                                impl_type.ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations with associated types. It first identifies traits with associated types and collects their names. Then, for each impl block, it checks if the trait being implemented is one of the identified candidates. If so, it modifies the associated type by randomly selecting a new type from existing top-level constructs in the file, such as structs, enums, traits, type aliases, or functions. This transformation aims to stress-test the compiler's handling of associated types by introducing unexpected but valid type substitutions, potentially revealing bugs in trait resolution and type inference."
    }
}