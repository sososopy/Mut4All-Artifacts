use crate::mutator::Mutator;
use syn::{parse_quote, Item, FnArg, PatType, Pat, Type, TypePath, ItemImpl};
use proc_macro2::Ident;
use std::collections::HashSet;

pub struct Mutator_Function_Signature_Mismatch_190;

impl Mutator for Mutator_Function_Signature_Mismatch_190 {
    fn name(&self) -> &str {
        "Mutator_Function_Signature_Mismatch_190"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_methods: HashSet<(Ident, Type)> = HashSet::new();

        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                for item in &trait_item.items {
                    if let syn::TraitItem::Fn(method) = item {
                        if let Some(FnArg::Typed(PatType { ty, pat, .. })) = method.sig.inputs.first() {
                            if let Pat::Ident(param_ident) = &*pat {
                                trait_methods.insert((param_ident.ident.clone(), (*ty).clone()));
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(ItemImpl { items, .. }) = item {
                for item in items {
                    if let syn::ImplItem::Fn(method) = item {
                        if let Some(FnArg::Typed(PatType { ty, pat, .. })) = method.sig.inputs.first_mut() {
                            if let Pat::Ident(param_ident) = &*pat {
                                if let Some((_, original_type)) = trait_methods.get(&param_ident.ident) {
                                    if let Type::Path(TypePath { path, .. }) = &*ty {
                                        if path.is_ident("u8") {
                                            *ty = parse_quote!(usize);
                                        } else if path.is_ident("usize") {
                                            *ty = parse_quote!(u8);
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
        "This mutator targets trait implementations in Rust programs. It identifies methods within trait definitions and their corresponding implementations. The mutation involves altering the parameter type in the implementation to a different valid type, specifically focusing on changing between types like `u8` and `usize`. This aims to introduce type mismatches that can reveal inconsistencies or trigger compiler errors due to ABI or layout issues."
    }
}