use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, ItemEnum, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Introduce_Const_Closure_With_Missing_Trait_Impl_326;

impl Mutator for Introduce_Const_Closure_With_Missing_Trait_Impl_326 {
    fn name(&self) -> &str {
        "Introduce_Const_Closure_With_Missing_Trait_Impl_326"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_const_trait_impl_feature = false;
        let mut has_const_closures_feature = false;
        for attr in &file.attrs {
            if let syn::Meta::List(list) = &attr.meta {
                for token in list.tokens.clone().into_iter() {
                    if let proc_macro2::TokenTree::Ident(ident) = token {
                        if ident == "const_trait_impl" {
                            has_const_trait_impl_feature = true;
                        }
                        if ident == "const_closures" {
                            has_const_closures_feature = true;
                        }
                    }
                }
            }
        }
        if !has_const_trait_impl_feature || !has_const_closures_feature {
            file.attrs.insert(0, parse_quote! {
                #![feature(const_trait_impl, const_closures)]
            });
        }

        let mut existing_traits = Vec::new();
        let mut existing_types = Vec::new();
        let mut trait_impl_pairs = HashSet::new();

        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                existing_traits.push(trait_item.ident.clone());
            }
            if let Item::Struct(struct_item) = item {
                existing_types.push(struct_item.ident.clone());
            }
            if let Item::Enum(enum_item) = item {
                existing_types.push(enum_item.ident.clone());
            }
            if let Item::Impl(impl_item) = item {
                if let Some((trait_path, self_ty)) = extract_trait_and_type(impl_item) {
                    trait_impl_pairs.insert((trait_path, self_ty));
                }
            }
        }

        let mut rng = thread_rng();
        let selected_trait = if !existing_traits.is_empty() && rng.gen_bool(0.5) {
            existing_traits.choose(&mut rng).cloned()
        } else {
            None
        };

        let (trait_ident, trait_method_ident) = if let Some(trait_ident) = selected_trait {
            (trait_ident, Ident::new("method", Span::call_site()))
        } else {
            let new_trait_ident = Ident::new("Trait", Span::call_site());
            let new_trait_item: Item = parse_quote! {
                trait Trait {
                    fn method(&self);
                }
            };
            file.items.insert(0, new_trait_item);
            (new_trait_ident, Ident::new("method", Span::call_site()))
        };

        let candidate_types: Vec<Ident> = existing_types
            .into_iter()
            .filter(|type_ident| !trait_impl_pairs.contains(&(trait_ident.clone().into(), type_ident.clone())))
            .collect();

        let selected_type = if !candidate_types.is_empty() && rng.gen_bool(0.5) {
            candidate_types.choose(&mut rng).cloned()
        } else {
            let new_type_ident = Ident::new("Concrete", Span::call_site());
            let new_struct_item: Item = parse_quote! {
                struct Concrete;
            };
            file.items.insert(1, new_struct_item);
            Some(new_type_ident)
        };

        if let Some(type_ident) = selected_type {
            let const_item: Item = parse_quote! {
                const _: () = {
                    (const || {
                        let _ = &#type_ident as &dyn #trait_ident;
                        (#trait_method_ident)(&(#type_ident));
                    })();
                };
            };
            file.items.push(const_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a const closure that attempts dynamic dispatch on a trait object where the concrete type does not implement the trait. It ensures the required features are enabled, reuses or creates necessary traits and types, and inserts a const item with a closure that casts to a dyn trait and calls a missing method. This aims to trigger compiler errors in const evaluation due to failed trait resolution."
    }
}

fn extract_trait_and_type(impl_item: &syn::ItemImpl) -> Option<(syn::Path, Ident)> {
    if let Some((_, trait_path, _)) = &impl_item.trait_ {
        if let syn::Type::Path(type_path) = &*impl_item.self_ty {
            if let Some(segment) = type_path.path.segments.last() {
                return Some((trait_path.clone(), segment.ident.clone()));
            }
        }
    }
    None
}