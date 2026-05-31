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

pub struct Introduce_Inherent_Associated_Type_With_Self_Reference_7;

impl Mutator for Introduce_Inherent_Associated_Type_With_Self_Reference_7 {
    fn name(&self) -> &str {
        "Introduce_Inherent_Associated_Type_With_Self_Reference_7"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_feature = false;
        for attr in &file.attrs {
            if attr.path.is_ident("feature") {
                if let Ok(syn::Meta::List(meta_list)) = attr.parse_meta() {
                    for nested in meta_list.nested.iter() {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested {
                            if path.is_ident("inherent_associated_types") {
                                has_feature = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if !has_feature {
            let feature_attr: syn::Attribute = parse_quote! {
                #![feature(inherent_associated_types)]
            };
            file.attrs.push(feature_attr);
        }
        let mut target_struct = None;
        for item in &file.items {
            if let syn::Item::Struct(item_struct) = item {
                target_struct = Some(item_struct.ident.clone());
                break;
            }
        }
        if let Some(target_struct) = target_struct {
            let mut has_inherent_associated_type = false;
            for item in &mut file.items {
                if let syn::Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        continue;
                    }
                    if let syn::Type::Path(type_path) = &*item_impl.self_ty {
                        if type_path.path.is_ident(&target_struct) {
                            for impl_item in &item_impl.items {
                                if let syn::ImplItem::Type(type_item) = impl_item {
                                    if type_item.ident == "P" {
                                        has_inherent_associated_type = true;
                                        break;
                                    }
                                }
                            }
                            if !has_inherent_associated_type {
                                let new_type: syn::ImplItem = parse_quote! {
                                    type P = Self::P;
                                };
                                item_impl.items.push(new_type);
                            }
                        }
                    }
                }
            }
        }
        let mut has_type_alias = false;
        for item in &file.items {
            if let syn::Item::Fn(item_fn) = item {
                if item_fn
                    .block
                    .stmts
                    .iter()
                    .any(|stmt| matches!(stmt, syn::Stmt::Item(syn::Item::Type(_))))
                {
                    has_type_alias = true;
                    break;
                }
            }
        }
        if !has_type_alias {
            let type_alias: syn::Stmt = parse_quote! {
                type A = S<i32>::P;
            };
            for item in &mut file.items {
                if let syn::Item::Fn(item_fn) = item {
                    item_fn.block.stmts.insert(0, type_alias.clone());
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets a struct in the seed program and introduces an inherent associated type within its impl block, defined as a self-reference. It ensures the necessary feature gate is active and verifies the presence of the new associated type by checking and modifying the impl block. Additionally, it injects a type alias in a function to utilize the new associated type, aiming to trigger compiler bugs related to self-referential type definitions."
    }
}