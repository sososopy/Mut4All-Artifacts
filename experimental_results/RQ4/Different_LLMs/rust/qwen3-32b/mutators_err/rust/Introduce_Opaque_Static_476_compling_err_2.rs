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

pub struct Introduce_Opaque_Static_476;

impl Mutator for Introduce_Opaque_Static_476 {
    fn name(&self) -> &str {
        "Introduce_Opaque_Static_476"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases: Vec<(Ident, String)> = vec![];
        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                let original_type_str = format!("{}", quote! { #item_type.ty });
                type_aliases.push((item_type.ident.clone(), original_type_str));
            }
        }

        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                let opaque_type = parse_quote!(impl Debug);
                item_type.ty = Box::new(opaque_type);
            }
        }

        let feature_name = "type_alias_impl_trait";
        let has_feature = file.attrs.iter().any(|attr| {
            if let syn::Meta::NameValue(meta) = &attr.meta {
                if meta.path.is_ident("feature") && !meta.path.leading_colon.is_some() {
                    if let syn::Lit::Str(lit) = &meta.value {
                        if lit.value() == feature_name {
                            return true;
                        }
                    }
                }
            }
            false
        });

        if !has_feature {
            let feature_attr = parse_quote!(#![feature(type_alias_impl_trait)]);
            file.attrs.insert(0, feature_attr);
        }

        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                let static_type_str = format!("{}", quote! { #item_static.ty });
                for (alias_name, original_type_str) in &type_aliases {
                    if static_type_str == *original_type_str {
                        item_static.ty = parse_quote!(#alias_name);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator converts type aliases into opaque types (impl Debug) and replaces static variables of the original type with the opaque alias. This introduces opaque types at the static level, potentially exposing issues in debug information and static variable handling. The transformation relies on the original type implementing Debug, which is assumed to be valid."
    }
}