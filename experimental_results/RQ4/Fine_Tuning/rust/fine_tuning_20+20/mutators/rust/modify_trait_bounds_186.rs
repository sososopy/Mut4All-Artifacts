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

pub struct Modify_Trait_Bounds_186;

impl Mutator for Modify_Trait_Bounds_186 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_186"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_trait_c = false;
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.ident == "TraitC" {
                    has_trait_c = true;
                    break;
                }
            }
        }
        if !has_trait_c {
            file.items
                .insert(0, parse_quote!(pub trait TraitC {}));
        }
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.ident != "TraitC" {
                    let mut has_trait_c_bound = false;
                    for bound in &trait_item.supertraits {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            if trait_bound.path.is_ident("TraitC") {
                                has_trait_c_bound = true;
                                break;
                            }
                        }
                    }
                    if !has_trait_c_bound {
                        trait_item
                            .supertraits
                            .push(parse_quote!(TraitC));
                    }
                }
            }
        }
        let mut struct_names = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                let struct_name = &struct_item.ident;
                let mut has_trait_c_impl = false;
                if struct_item
                    .attrs
                    .iter()
                    .any(|attr| attr.path().is_ident("derive"))
                {
                    if struct_item
                        .attrs
                        .iter()
                        .any(|attr| attr.path().is_ident("TraitC"))
                    {
                        has_trait_c_impl = true;
                    }
                }
                if !has_trait_c_impl {
                    struct_names.push(struct_name.clone());
                }
            }
        }
        for struct_name in struct_names {
            file.items.push(parse_quote! {
                impl TraitC for #struct_name {}
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits with existing bounds by introducing a new unrelated trait, `TraitC`, and modifying the trait to include `TraitC` in its bounds. It ensures that any types implementing the original trait also implement `TraitC`, either by modifying existing implementations or adding new ones. This transformation stresses the trait system and type checker by altering trait hierarchies and enforcing additional constraints, potentially leading to ICEs in layout computation and trait resolution."
    }
}