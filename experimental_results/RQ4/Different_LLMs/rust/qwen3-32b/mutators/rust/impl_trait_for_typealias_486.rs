use rand::prelude::SliceRandom;
use syn::parse_quote;
use crate::mutator::Mutator;
use rand::thread_rng;

pub struct Impl_Trait_For_TypeAlias_486;

impl Mutator for Impl_Trait_For_TypeAlias_486 {
    fn name(&self) -> &str {
        "Impl_Trait_For_TypeAlias_486"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits = vec![];
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                traits.push(trait_item.clone());
            }
        }

        if traits.is_empty() {
            return;
        }

        let mut insertions = vec![];
        for (i, item) in file.items.iter().enumerate() {
            if let syn::Item::Type(item_type) = item {
                if let syn::Type::ImplTrait(_) = &*item_type.ty {
                    insertions.push((i, item_type.ident.clone()));
                }
            }
        }

        if insertions.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        for &(i, ref alias_ident) in insertions.iter().rev() {
            let selected_trait = traits.choose(&mut rng).unwrap();
            let trait_path = selected_trait.ident.clone();
            let new_impl = parse_quote! {
                impl #trait_path for #alias_ident {}
            };
            file.items.insert(i + 1, syn::Item::Impl(new_impl));
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}