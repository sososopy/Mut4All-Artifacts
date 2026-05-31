use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Type_With_Alias_288;

impl Mutator for Replace_Const_Type_With_Alias_288 {
    fn name(&self) -> &str {
        "Replace_Const_Type_With_Alias_288"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut existing_aliases = HashSet::new();
        let mut const_params_to_mutate = Vec::new();
        let mut alias_insertion_points = Vec::new();

        for (item_idx, item) in file.items.iter().enumerate() {
            match item {
                Item::Type(item_type) => {
                    if let Type::Path(type_path) = &*item_type.ty {
                        existing_aliases.insert(type_path.path.segments.last().unwrap().ident.to_string());
                    }
                }
                Item::Mod(item_mod) => {
                    if item_mod.content.is_none() {
                        continue;
                    }
                    for (_, sub_item) in item_mod.content.as_ref().unwrap().1.iter().enumerate() {
                        if let Item::Type(sub_type) = sub_item {
                            if let Type::Path(type_path) = &*sub_type.2 {
                                existing_aliases.insert(type_path.path.segments.last().unwrap().ident.to_string());
                            }
                        }
                    }
                }
                _ => {}
            }
        }

        for (item_idx, item) in file.items.iter_mut().enumerate() {
            match item {
                Item::Fn(item_fn) => {
                    for (param_idx, param) in item_fn.sig.generics.params.iter().enumerate() {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(type_path) = &const_param.ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    let type_name = segment.ident.to_string();
                                    if matches!(type_name.as_str(), "i32" | "u32" | "i64" | "u64" | "usize" | "isize" | "bool" | "char") {
                                        const_params_to_mutate.push((item_idx, param_idx, type_name.clone()));
                                    }
                                }
                            }
                        }
                    }
                }
                Item::Struct(item_struct) => {
                    for (param_idx, param) in item_struct.generics.params.iter().enumerate() {
                        if let GenericParam2::Const(const_param) = param {
                            if let Type::Path(type_path) = &const_param.ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    let type_name = segment.ident.to_string();
                                    if matches!(type_name.as_str(), "i32" | "u32" | "i64" | "u64" | "usize" | "isize" | "bool" | "char") {
                                        const_params_to_mutate.push((item_idx, param_idx, type_name.clone()));
                                    }
                                }
                            }
                        }
                    }
                }
                Item::Impl(item_impl) => {
                    for (param_idx, param) in item_impl.generics.params.iter().enumerate() {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(type_path) = &const_param.ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    let type_name = segment.ident.to_string();
                                    if matches!(type_name.as_str(), "i32" | "u32" | "i64" | "u64" | "usize" | "isize" | "bool" | "char") {
                                        const_params_to_mutate.push((item_idx, param_idx, type_name.clone()));
                                    }
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }

        if const_params_to_mutate.is_empty() {
            return;
        }

        let (item_idx, param_idx, original_type) = const_params_to_mutate.choose(&mut rng).unwrap().clone();
        let alias_name = generate_unique_alias_name(&original_type, &existing_aliases, &mut rng);
        let target_type = get_different_type(&original_type, &mut rng);

        alias_insertion_points.push((item_idx, alias_name.clone(), target_type.clone()));

        match &mut file.items[item_idx] {
            Item::Fn(item_fn) => {
                if let GenericParam::Const(const_param) = &mut item_fn.sig.generics.params[param_idx] {
                    const_param.ty = parse_quote!(#alias_name);
                }
            }
            Item::Struct(item_struct) => {
                if let GenericParam::Const(const_param) = &mut item_struct.generics.params[param_idx] {
                    const_param.ty = parse_quote!(#alias_name);
                }
            }
            Item::Impl(item_impl) => {
                if let GenericParam::Const(const_param) = &mut item_impl.generics.params[param_idx] {
                    const_param.ty = parse_quote!(#alias_name);
                }
            }
            _ => unreachable!(),
        }

        for (insert_idx, alias_name, target_type) in alias_insertion_points {
            let new_alias: Item = parse_quote! {
                type #alias_name = #target_type;
            };
            file.items.insert(insert_idx, new_alias);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters with concrete types (i32, u32, i64, u64, usize, isize, bool, char) in functions, structs, and impls. It replaces the concrete type with a type alias that resolves to a different but compatible type (e.g., i32 -> u32). If no suitable alias exists, it creates a new unique type alias at the appropriate scope. The constant argument values remain unchanged, potentially causing type mismatches during constant evaluation or triggering internal compiler errors in const generic resolution, especially with adt_const_params."
    }
}

fn generate_unique_alias_name(original_type: &str, existing_aliases: &HashSet<String>, rng: &mut impl Rng) -> Ident {
    let mut candidate = format!("AliasFor{}", original_type.to_uppercase());
    let mut suffix = 1;
    while existing_aliases.contains(&candidate) {
        candidate = format!("AliasFor{}_{}", original_type.to_uppercase(), suffix);
        suffix += 1;
    }
    if suffix > 1 && rng.gen_bool(0.3) {
        candidate = format!("MutatedType_{}", rng.gen_range(1000..9999));
    }
    Ident::new(&candidate, Span::call_site())
}

fn get_different_type(original_type: &str, rng: &mut impl Rng) -> String {
    let candidates = match original_type {
        "i32" => vec!["u32", "i64", "isize"],
        "u32" => vec!["i32", "u64", "usize"],
        "i64" => vec!["u64", "i32", "isize"],
        "u64" => vec!["i64", "u32", "usize"],
        "usize" => vec!["isize", "u32", "u64"],
        "isize" => vec!["usize", "i32", "i64"],
        "bool" => vec!["char", "u8"],
        "char" => vec!["bool", "u32"],
        _ => vec!["u32", "i32", "usize"],
    };
    candidates.choose(rng).unwrap().to_string()
}