use syn::parse_quote;
use crate::mutator::Mutator;
use std::collections::HashSet;

pub struct Add_Conflicting_Trait_Impl_263;

impl Mutator for Add_Conflicting_Trait_Impl_263 {
    fn name(&self) -> &str {
        "Add_Conflicting_Trait_Impl_263"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_traits = Vec::new();
        let mut processed_traits = HashSet::new();

        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                let trait_ident = &trait_def.ident;
                for trait_item in &trait_def.items {
                    if let syn::TraitItem::Type(trait_type) = trait_item {
                        if !trait_type.bounds.is_empty() {
                            target_traits.push((
                                trait_ident.clone(),
                                trait_type.ident.clone(),
                                trait_type.bounds.clone(),
                            ));
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    if let Some(last_segment) = trait_path.segments.last() {
                        for (trait_ident, associated_type_name, _) in &target_traits {
                            if last_segment.ident == *trait_ident {
                                if !processed_traits.contains(trait_ident) {
                                    let new_impl = parse_quote! {
                                        impl #trait_path for i32 {
                                            type #associated_type_name = i32;
                                        }
                                    };
                                    file.items.push(syn::Item::Impl(new_impl));
                                    processed_traits.insert(trait_ident.clone());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}