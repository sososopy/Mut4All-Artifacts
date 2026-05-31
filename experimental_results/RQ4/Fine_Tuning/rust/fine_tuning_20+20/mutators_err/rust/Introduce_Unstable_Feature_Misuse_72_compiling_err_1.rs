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

pub struct Introduce_Unstable_Feature_Misuse_72;

impl Mutator for Introduce_Unstable_Feature_Misuse_72 {
    fn name(&self) -> &str {
        "Introduce_Unstable_Feature_Misuse_72"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_arbitrary_enum_discriminant = false;

        for attr in &file.attrs {
            if attr.path.is_ident("feature") {
                if let Ok(syn::Meta::List(meta_list)) = attr.parse_meta() {
                    for nested_meta in meta_list.nested.iter() {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested_meta {
                            if path.is_ident("arbitrary_enum_discriminant") {
                                has_arbitrary_enum_discriminant = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if !has_arbitrary_enum_discriminant {
            file.attrs.push(parse_quote!(#![feature(arbitrary_enum_discriminant)]));
        }

        let mut enum_indices = vec![];

        for (index, item) in file.items.iter().enumerate() {
            if let Item::Enum(item_enum) = item {
                enum_indices.push(index);
            }
        }

        if enum_indices.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let enum_index = enum_indices.choose(&mut rng).unwrap();

        if let Item::Enum(item_enum) = &mut file.items[*enum_index] {
            let mut has_custom_discriminant = false;

            for variant in &item_enum.variants {
                if variant.discriminant.is_some() {
                    has_custom_discriminant = true;
                    break;
                }
            }

            if !has_custom_discriminant {
                let mut discriminant_value = 1;
                for variant in &mut item_enum.variants {
                    variant.discriminant = Some((
                        token::Eq::default(),
                        Expr::Lit(ExprLit {
                            attrs: Vec::new(),
                            lit: syn::Lit::Int(syn::LitInt::new(
                                &discriminant_value.to_string(),
                                Span::call_site(),
                            )),
                        }),
                    ));
                    discriminant_value += 2;
                }
            }
        }

        let new_enum: ItemEnum = parse_quote! {
            enum Faulty {
                C = 0,
                D(!),
            }
        };

        file.items.push(Item::Enum(new_enum));

        let usage_fn: ItemFn = parse_quote! {
            fn usage() {
                let _ = Faulty::D;
            }
        };

        file.items.push(Item::Fn(usage_fn));
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enum definitions with custom discriminants and introduces misuse of unstable features. It adds the `#![feature(arbitrary_enum_discriminant)]` attribute if not present, modifies an existing enum to ensure it has custom discriminants, and injects a new enum with a variant using the never type (`!`) without enabling the necessary feature. By leveraging improper use of the `!` type in conjunction with custom discriminants and unstable features, the mutator is designed to trigger compiler bugs related to const evaluation and type checking logic."
    }
}