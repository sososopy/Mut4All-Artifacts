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

pub struct Mutate_Impl_Method_Param_Type_389;

impl Mutator for Mutate_Impl_Method_Param_Type_389 {
    fn name(&self) -> &str {
        "Mutate_Impl_Method_Param_Type_389"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_map: HashMap<String, syn::ItemTrait> = HashMap::new();
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                trait_map.insert(trait_item.ident.to_string(), trait_item.clone());
            }
        }
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    let trait_name = path
                        .segments
                        .last()
                        .expect("Expected at least one segment in the path")
                        .ident
                        .to_string();
                    if let Some(trait_item) = trait_map.get(&trait_name) {
                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Fn(impl_fn) = impl_item {
                                let impl_fn_name = impl_fn.sig.ident.to_string();
                                for trait_item in &trait_item.items {
                                    if let syn::TraitItem::Fn(trait_fn) = trait_item {
                                        if trait_fn.sig.ident.to_string() == impl_fn_name {
                                            for (impl_arg, trait_arg) in impl_fn
                                                .sig
                                                .inputs
                                                .iter_mut()
                                                .zip(trait_fn.sig.inputs.iter())
                                            {
                                                if let (syn::FnArg::Typed(impl_pat_type), syn::FnArg::Typed(trait_pat_type)) =
                                                    (impl_arg, trait_arg)
                                                {
                                                    if let syn::Type::Path(syn::TypePath {
                                                        qself: _,
                                                        path,
                                                    }) = &*trait_pat_type.ty
                                                    {
                                                        let original_type = path
                                                            .segments
                                                            .last()
                                                            .expect(
                                                                "Expected at least one segment in the path",
                                                            )
                                                            .ident
                                                            .to_string();
                                                        let replacement_type = match original_type
                                                            .as_str()
                                                        {
                                                            "u8" => "usize",
                                                            "usize" => "u8",
                                                            "i8" => "isize",
                                                            "isize" => "i8",
                                                            "u16" => "u32",
                                                            "u32" => "u16",
                                                            "i16" => "i32",
                                                            "i32" => "i16",
                                                            "u64" => "u128",
                                                            "u128" => "u64",
                                                            "i64" => "i128",
                                                            "i128" => "i64",
                                                            _ => continue,
                                                        };
                                                        impl_pat_type.ty = Box::new(
                                                            syn::Type::Path(syn::TypePath {
                                                                qself: None,
                                                                path: syn::Path::from(
                                                                    syn::Ident::new(
                                                                        replacement_type,
                                                                        proc_macro2::Span::call_site(
                                                                        ),
                                                                    ),
                                                                ),
                                                            }),
                                                        );
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations by altering the parameter types of methods to mismatch those in the trait definition. It replaces types with others that differ in bit size, such as changing `u8` to `usize`, to exploit potential compiler weaknesses in type checking and ABI handling for trait methods."
    }
}