use proc_macro2::{Span, *};
use quote::*;
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};
use crate::mutator::Mutator;

pub struct Specialized_Trait_Implementation_Mutation_28;

impl Mutator for Specialized_Trait_Implementation_Mutation_28 {
    fn name(&self) -> &str {
        "Specialized_Trait_Implementation_Mutation_28"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut found_default = false;
                let mut trait_path = None;
                
                for impl_item in &item_impl.items {
                    if let syn::ImplItem::Type(impl_type) = impl_item {
                        if impl_type.defaultness.is_some() {
                            found_default = true;
                            trait_path = item_impl.trait_.as_ref().map(|(_, path, _)| path.clone());
                        }
                    }
                }

                if found_default {
                    if let Some(trait_path) = trait_path {
                        let specialized_impl: syn::Item = parse_quote! {
                            impl #trait_path for i32 {
                                type Associated = Option<i32>;
                            }
                        };
                        new_items.push(specialized_impl);
                    }
                }
            }
        }
        
        file.items.extend(new_items);
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations that use `default` for associated types. It adds a specialized implementation for `i32`, changing the associated type to `Option<i32>`. This mutation is designed to explore potential issues with Rust's specialization feature by introducing a conflict in associated type resolution."
    }
}