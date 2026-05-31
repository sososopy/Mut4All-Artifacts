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

pub struct Add_Const_Generic_Trait_Impl_98;

impl Mutator for Add_Const_Generic_Trait_Impl_98 {
    fn name(&self) -> &str {
        "Add_Const_Generic_Trait_Impl_98"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_trait = false;
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                has_trait = true;
                break;
            }
        }
        if !has_trait {
            return;
        }
        let mut has_target_trait = false;
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item
                    .items
                    .iter()
                    .any(|i| matches!(i, TraitItem::Const(_)))
                {
                    has_target_trait = true;
                    break;
                }
            }
        }
        if !has_target_trait {
            return;
        }
        let mut has_target_impl = false;
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl
                    .items
                    .iter()
                    .any(|i| matches!(i, ImplItem::Const(_)))
                {
                    has_target_impl = true;
                    break;
                }
            }
        }
        if !has_target_impl {
            return;
        }
        let mut has_target_fn = false;
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Array(type_array) = &**return_type {
                        if let Expr::Path(expr_path) = &type_array.len {
                            if expr_path
                                .path
                                .segments
                                .iter()
                                .any(|seg| seg.ident == "ASSOC")
                            {
                                has_target_fn = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if !has_target_fn {
            return;
        }
        let mut rng = thread_rng();
        let mut trait_name = format!("T{}", rng.r#gen::<u32>());
        let mut used_names = HashSet::new();
        for item in &file.items {
            match item {
                Item::Struct(item_struct) => {
                    used_names.insert(item_struct.ident.to_string());
                }
                Item::Enum(item_enum) => {
                    used_names.insert(item_enum.ident.to_string());
                }
                Item::Union(item_union) => {
                    used_names.insert(item_union.ident.to_string());
                }
                Item::Trait(item_trait) => {
                    used_names.insert(item_trait.ident.to_string());
                }
                Item::Impl(item_impl) => {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if let Some(seg) = path.segments.last() {
                            used_names.insert(seg.ident.to_string());
                        }
                    }
                }
                Item::Fn(item_fn) => {
                    used_names.insert(item_fn.sig.ident.to_string());
                }
                _ => {}
            }
        }
        while used_names.contains(&trait_name) {
            trait_name = format!("T{}", rng.r#gen::<u32>());
        }
        let mut impl_name = format!("I{}", rng.r#gen::<u32>());
        while used_names.contains(&impl_name) {
            impl_name = format!("I{}", rng.r#gen::<u32>());
        }
        let mut const_name = format!("C{}", rng.r#gen::<u32>());
        while used_names.contains(&const_name) {
            const_name = format!("C{}", rng.r#gen::<u32>());
        }
        let mut fn_name = format!("f{}", rng.r#gen::<u32>());
        while used_names.contains(&fn_name) {
            fn_name = format!("f{}", rng.r#gen::<u32>());
        }
        let trait_item: Item = parse_quote! {
            trait #trait_name {
                const #const_name: usize;
            }
        };
        let impl_item: Item = parse_quote! {
            impl<const M: u64> #trait_name for [u8; M] {
                const #const_name: usize = M as usize;
            }
        };
        let fn_item: Item = parse_quote! {
            fn #fn_name() -> [u8; <[u8; 13] as #trait_name>::#const_name] {
                []
            }
        };
        file.items.push(trait_item);
        file.items.push(impl_item);
        file.items.push(fn_item);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator introduces a new trait with an associated constant and implements it using constant generics. It then defines a function that returns an array whose size is determined by the trait's associated constant, leveraging the constant generic parameter. This transformation is designed to stress-test the compiler's handling of constant generics, associated constants, and trait resolution, potentially exposing weaknesses in const evaluation and type inference."
    }
}