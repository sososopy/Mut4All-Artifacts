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
use std::collections::HashMap;

use crate::mutator::Mutator;

pub struct Enum_With_Exotic_Discriminant_61;

impl Mutator for Enum_With_Exotic_Discriminant_61 {
    fn name(&self) -> &str {
        "Enum_With_Exotic_Discriminant_61"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut enum_names = Vec::new();
        let mut enum_discriminant_map = HashMap::new();
        for item in &file.items {
            if let Item::Enum(item_enum) = item {
                let mut discriminant_values = Vec::new();
                for variant in &item_enum.variants {
                    if let Some((_, expr)) = &variant.discriminant {
                        discriminant_values.push(expr.clone());
                    }
                }
                if !discriminant_values.is_empty() {
                    enum_names.push(item_enum.ident.clone());
                    enum_discriminant_map.insert(item_enum.ident.clone(), discriminant_values);
                }
            }
        }
        if enum_names.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let selected_enum_name = enum_names.choose(&mut rng).unwrap();
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if &item_enum.ident == selected_enum_name {
                    let discriminant_values = enum_discriminant_map.get(selected_enum_name).unwrap();
                    let selected_discriminant = discriminant_values.choose(&mut rng).unwrap();
                    let exotic_discriminant: Expr = parse_quote! { (-1isize) as usize };
                    for variant in &mut item_enum.variants {
                        if let Some((_, expr)) = &mut variant.discriminant {
                            if expr == selected_discriminant {
                                *expr = exotic_discriminant.clone();
                            }
                        }
                    }
                }
            }
        }
        let mut const_names = Vec::new();
        let mut const_expr_map = HashMap::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                let mut found = false;
                if let Type::Path(type_path) = &*item_const.ty {
                    if type_path.path.segments.len() == 1 {
                        let segment = type_path.path.segments.first().unwrap();
                        if enum_names.contains(&segment.ident) {
                            found = true;
                        }
                    }
                }
                if found {
                    const_names.push(item_const.ident.clone());
                    const_expr_map.insert(item_const.ident.clone(), *item_const.expr.clone());
                }
            }
        }
        if const_names.is_empty() {
            return;
        }
        let selected_const_name = const_names.choose(&mut rng).unwrap();
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if &item_const.ident == selected_const_name {
                    let original_expr = const_expr_map.get(selected_const_name).unwrap();
                    let exotic_expr: Expr = parse_quote! { 1024usize };
                    if original_expr != &exotic_expr {
                        *item_const.expr = exotic_expr;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enums with explicit discriminants and modifies one of the discriminants to use a non-trivial or invalid expression, such as a negative number cast to usize. It also ensures that the enum is used in a constant context, replacing the constant's value with an out-of-bounds index to stress the compiler's handling of enum layout and constant evaluation."
    }
}