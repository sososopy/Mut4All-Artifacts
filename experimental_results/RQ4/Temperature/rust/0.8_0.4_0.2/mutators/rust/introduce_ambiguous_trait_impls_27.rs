use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Introduce_Ambiguous_Trait_Impls_27;

impl Mutator for Introduce_Ambiguous_Trait_Impls_27 {
    fn name(&self) -> &str {
        "Introduce_Ambiguous_Trait_Impls_27"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        let mut opaque_impls = Vec::new();
        
        let items_clone = file.items.clone(); // Clone the items for immutable access

        for item in &mut file.items {
            if let syn::Item::Trait(item_trait) = item {
                let new_trait_ident = Ident::new(&format!("Ambiguous{}", item_trait.ident), Span::call_site());
                let new_trait: Item = parse_quote! {
                    trait #new_trait_ident {}
                };
                new_items.push(new_trait);

                let impl_send: Item = parse_quote! {
                    impl<T: Send> #new_trait_ident for T {}
                };
                let impl_sized: Item = parse_quote! {
                    impl<T: Sized> #new_trait_ident for T {}
                };
                new_items.push(impl_send);
                new_items.push(impl_sized);

                if let Some(opaque_impl) = find_opaque_impl(&items_clone, &item_trait.ident) {
                    opaque_impls.push((new_trait_ident.clone(), opaque_impl));
                }
            }
        }

        for (new_trait_ident, opaque_impl) in opaque_impls {
            let new_opaque_impl: Item = parse_quote! {
                impl #new_trait_ident for #opaque_impl {}
            };
            new_items.push(new_opaque_impl);
        }

        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a new trait with ambiguous implementations for any type `T` with different bounds (`Send` and `Sized`). This setup challenges the compiler's trait resolution and specialization logic, potentially exposing bugs related to coherence and ambiguity in trait implementations."
    }
}

fn find_opaque_impl(items: &[Item], trait_ident: &Ident) -> Option<Ident> {
    for item in items {
        if let syn::Item::Impl(item_impl) = item {
            if let Some((_, path, _)) = &item_impl.trait_ {
                if path.is_ident(trait_ident) {
                    if let syn::Type::Path(type_path) = &*item_impl.self_ty {
                        if let Some(segment) = type_path.path.segments.last() {
                            return Some(segment.ident.clone());
                        }
                    }
                }
            }
        }
    }
    None
}