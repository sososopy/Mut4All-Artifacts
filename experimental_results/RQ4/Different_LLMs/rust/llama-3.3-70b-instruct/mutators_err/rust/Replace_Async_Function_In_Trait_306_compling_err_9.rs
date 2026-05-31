use syn::{Item, TraitItem};
use syn::TraitItem;
use crate::mutator::Mutator;

struct Replace_Async_Function_In_Trait_306;

impl Mutator for Replace_Async_Function_In_Trait_306 {
    fn name(&self) -> &str {
        "Replace_Async_Function_In_Trait_306"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let TraitItem::Method(method) = item {
                        if method.sig.asyncness.is_some() {
                            // Replace async function with non-async function
                            method.sig.asyncness = None;
                        } else {
                            // Replace non-async function with async function
                            method.sig.asyncness = Some(syn::token::Async::default());
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces an async function in a trait with a non-async function and vice versa, to test how the Rust compiler handles the change in the function signature, especially when the trait is used in an async context."
    }
}

impl Replace_Async_Function_In_Trait_306 {
    pub fn new() -> Self {
        Replace_Async_Function_In_Trait_306
    }
}