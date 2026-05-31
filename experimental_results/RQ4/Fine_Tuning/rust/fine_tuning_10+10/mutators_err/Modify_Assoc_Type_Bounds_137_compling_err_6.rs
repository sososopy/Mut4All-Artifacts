use crate::mutator::Mutator;
use syn::parse_quote;
use syn::Item;

pub struct ModifyAssocTypeBounds137;

impl Mutator for ModifyAssocTypeBounds137 {
    fn name(&self) -> &str {
        "ModifyAssocTypeBounds137"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                trait_item.attrs.push(parse_quote!(#[const_trait]));
                let trait_ident = trait_item.ident.clone();
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(assoc_type) = item {
                        assoc_type.bounds.push(parse_quote!(#trait_ident));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait definitions with associated types, modifying the associated type bounds to introduce recursive trait bounds. It identifies traits with associated types, adds a const_trait attribute to the trait, and updates the associated type bounds to include the trait itself as a bound. This tests the compiler's handling of recursive trait bounds."
    }
}