use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, ItemImpl, ImplItem, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Introduce_Trait_For_Specialized_Associated_Type_454;

impl Mutator for Introduce_Trait_For_Specialized_Associated_Type_454 {
    fn name(&self) -> &str {
        "Introduce_Trait_For_Specialized_Associated_Type_454"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut default_impls_with_assoc = Vec::new();
        let mut specialized_impls = Vec::new();
        let mut existing_traits = HashSet::new();

        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                existing_traits.insert(item_trait.ident.to_string());
            }
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_some() {
                    let trait_path = item_impl.trait_.as_ref().unwrap().1.clone();
                    for impl_item in &item_impl.items {
                        if let ImplItem::Type(assoc_type) = impl_item {
                            if item_impl.generics.params.is_empty() && item_impl.self_ty == parse_quote!(T) {
                                continue;
                            }
                            if let Some(default_token) = &item_impl.defaultness {
                                default_impls_with_assoc.push((trait_path.clone(), assoc_type.ident.clone(), item_impl.self_ty.clone()));
                            } else {
                                specialized_impls.push((trait_path.clone(), assoc_type.ident.clone(), item_impl.self_ty.clone(), assoc_type.ty.clone()));
                            }
                        }
                    }
                }
            }
        }

        for (trait_path, assoc_name, self_ty, assoc_ty) in &specialized_impls {
            if default_impls_with_assoc.iter().any(|(def_trait, def_assoc, def_self)| {
                def_trait == trait_path && def_assoc == assoc_name && def_self != self_ty
            }) {
                let target_trait_name = if existing_traits.contains("Trait2") {
                    "Trait2"
                } else if existing_traits.contains("NewTrait") {
                    "NewTrait"
                } else {
                    "Trait2"
                };

                if !existing_traits.contains(target_trait_name) {
                    let new_trait: ItemTrait = parse_quote! {
                        trait #target_trait_name {}
                    };
                    file.items.push(Item::Trait(new_trait));
                    existing_traits.insert(target_trait_name.to_string());
                }

                let assoc_type_path: TypePath = parse_quote! {
                    <#self_ty as #trait_path>::#assoc_name
                };
                let new_impl: ItemImpl = parse_quote! {
                    impl #target_trait_name for #assoc_type_path {}
                };
                file.items.push(Item::Impl(new_impl));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets specialization scenarios where a trait has a default implementation with an associated type and a more specific implementation for a concrete type that overrides that associated type. It introduces a new trait (or reuses an existing one) and implements it for the associated type from the specialized impl. This creates normalization cycles and intercrate assertion failures by forcing the compiler to resolve the associated type while building the specialization graph."
    }
}