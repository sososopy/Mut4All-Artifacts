use proc_macro2::TokenStream;
use quote::quote;
use syn::{Item, ItemTrait, parse_file};

struct Create_Trait_Cyclic_Dependency_146;

impl Create_Trait_Cyclic_Dependency_146 {
    fn mutate(&self, ast: &mut syn::File) {
        let mut trait_items: Vec<&mut ItemTrait> = Vec::new();
        for item in &mut ast.items {
            if let Item::Trait(trait_item) = item {
                trait_items.push(trait_item);
            }
        }

        if trait_items.len() >= 2 {
            let first_trait = &trait_items[0];
            let second_trait = &trait_items[1];
            
            let first_ident = &first_trait.ident;
            let second_ident = &second_trait.ident;

            for trait_item in trait_items.iter_mut() {
                if trait_item.ident == *first_ident {
                    let new_item: Item = syn::parse_quote! {
                        trait #first_ident: #second_ident {}
                    };
                    *trait_item = if let Item::Trait(t) = new_item { t } else { unreachable!() };
                } else if trait_item.ident == *second_ident {
                    let new_item: Item = syn::parse_quote! {
                        trait #second_ident: #first_ident {}
                    };
                    *trait_item = if let Item::Trait(t) = new_item { t } else { unreachable!() };
                }
            }
        }
    }
}