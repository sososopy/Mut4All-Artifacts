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

pub struct Modify_Const_Array_Type_67;

fn find_compatible_trait(ty: &Type, file: &syn::File) -> Option<String> {
    let mut compatible_traits = HashSet::new();
    let mut trait_collector = |item: &syn::Item| {
        if let syn::Item::Trait(trait_item) = item {
            compatible_traits.insert(trait_item.ident.to_string());
        }
    };
    for item in &file.items {
        trait_collector(item);
    }
    let mut visitor = |ty: &syn::Type| {
        if let syn::Type::Path(type_path) = ty {
            if let Some(segment) = type_path.path.segments.last() {
                if segment.ident == "dyn" {
                    if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                        for arg in &args.args {
                            if let syn::GenericArgument::Type(arg_ty) = arg {
                                if let syn::Type::Path(arg_type_path) = arg_ty {
                                    if let Some(arg_segment) =
                                        arg_type_path.path.segments.last()
                                    {
                                        compatible_traits.remove(&arg_segment.ident.to_string());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    };
    visitor(ty);
    let mut impl_collector = |item: &syn::Item| {
        if let syn::Item::Impl(impl_item) = item {
            if let Some((_, path, _)) = &impl_item.trait_ {
                if let Some(segment) = path.segments.last() {
                    if compatible_traits.contains(&segment.ident.to_string()) {
                        if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                            if let Some(self_segment) = type_path.path.segments.last() {
                                if self_segment.ident == "dyn" {
                                    if let syn::PathArguments::AngleBracketed(args) =
                                        &self_segment.arguments
                                    {
                                        for arg in &args.args {
                                            if let syn::GenericArgument::Type(arg_ty) = arg {
                                                if let syn::Type::Path(arg_type_path) = arg_ty {
                                                    if let Some(arg_segment) =
                                                        arg_type_path.path.segments.last()
                                                    {
                                                        if arg_segment.ident == "dyn" {
                                                            if let syn::PathArguments::AngleBracketed(
                                                                inner_args,
                                                            ) = &arg_segment.arguments
                                                            {
                                                                for inner_arg in &inner_args.args {
                                                                    if let syn::GenericArgument::Type(
                                                                        inner_arg_ty,
                                                                    ) = inner_arg
                                                                    {
                                                                        if let syn::Type::Path(
                                                                            inner_arg_type_path,
                                                                        ) = inner_arg_ty
                                                                        {
                                                                            if let Some(
                                                                                inner_arg_segment,
                                                                            ) = inner_arg_type_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                            {
                                                                                compatible_traits.remove(
                                                                                    &inner_arg_segment
                                                                                        .ident
                                                                                        .to_string(),
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
                        }
                    }
                }
            }
        }
    };
    for item in &file.items {
        impl_collector(item);
    }
    let mut trait_impl_collector = |item: &syn::Item| {
        if let syn::Item::Impl(impl_item) = item {
            if impl_item.trait_.is_none() {
                if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                    if let Some(segment) = type_path.path.segments.last() {
                        if segment.ident == "dyn" {
                            if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                                for arg in &args.args {
                                    if let syn::GenericArgument::Type(arg_ty) = arg {
                                        if let syn::Type::Path(arg_type_path) = arg_ty {
                                            if let Some(arg_segment) =
                                                arg_type_path.path.segments.last()
                                            {
                                                compatible_traits.remove(
                                                    &arg_segment.ident.to_string(),
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
    };
    for item in &file.items {
        trait_impl_collector(item);
    }
    let mut inherent_impl_collector = |item: &syn::Item| {
        if let syn::Item::Impl(impl_item) = item {
            if impl_item.trait_.is_none() {
                if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                    if let Some(segment) = type_path.path.segments.last() {
                        compatible_traits.remove(&segment.ident.to_string());
                    }
                }
            }
        }
    };
    for item in &file.items {
        inherent_impl_collector(item);
    }
    let mut struct_collector = |item: &syn::Item| {
        if let syn::Item::Struct(struct_item) = item {
            compatible_traits.remove(&struct_item.ident.to_string());
        }
    };
    for item in &file.items {
        struct_collector(item);
    }
    let mut enum_collector = |item: &syn::Item| {
        if let syn::Item::Enum(enum_item) = item {
            compatible_traits.remove(&enum_item.ident.to_string());
        }
    };
    for item in &file.items {
        enum_collector(item);
    }
    let mut type_alias_collector = |item: &syn::Item| {
        if let syn::Item::Type(type_item) = item {
            compatible_traits.remove(&type_item.ident.to_string());
        }
    };
    for item in &file.items {
        type_alias_collector(item);
    }
    let mut trait_alias_collector = |item: &syn::Item| {
        if let syn::Item::TraitAlias(trait_alias_item) = item {
            compatible_traits.remove(&trait_alias_item.ident.to_string());
        }
    };
    for item in &file.items {
        trait_alias_collector(item);
    }
    compatible_traits.iter().next().cloned()
}

fn transform_const_array_type(ty: &mut Type, file: &syn::File) {
    if let syn::Type::Reference(type_reference) = ty {
        if let syn::Type::Slice(type_slice) = &*type_reference.elem {
            if let Some(trait_name) = find_compatible_trait(&type_slice.elem, file) {
                let trait_object_type: syn::Type = parse_quote! { dyn #trait_name };
                let new_slice_type = syn::Type::Slice(syn::TypeSlice {
                    bracket_token: type_slice.bracket_token,
                    elem: Box::new(trait_object_type),
                });
                type_reference.elem = Box::new(syn::Type::Slice(new_slice_type));
            }
        }
    }
}

impl Mutator for Modify_Const_Array_Type_67 {
    fn name(&self) -> &str {
        "Modify_Const_Array_Type_67"
    }
    fn mutate(&self, file: &mut syn::File) {
        let items = file.items.clone();
        for item in items {
            if let syn::Item::Const(mut item_const) = item {
                if let syn::Type::Reference(type_reference) = &*item_const.ty {
                    if let syn::Type::Slice(type_slice) = &*type_reference.elem {
                        transform_const_array_type(&mut *item_const.ty, file);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets constant array declarations by changing their element type to a trait object type. It identifies const arrays and attempts to replace their element type with a compatible trait object, leveraging existing traits implemented by the element type. This transformation increases type complexity and challenges the compiler's const evaluation and trait resolution mechanisms, aiming to expose bugs related to type coercion and trait object handling in constant contexts."
    }
}