use crate::mutator::Mutator;

pub struct Modify_Trait_Definition_And_Implementation_125;

impl Mutator for Modify_Trait_Definition_And_Implementation_125 {
    fn name(&self) -> &str {
        "Modify_Trait_Definition_And_Implementation_125"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_items = Vec::new();
        let mut impl_items = Vec::new();

        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let new_method_ident = Ident::new("new_method", Span::call_site());
                let new_method_sig: syn::TraitItem = parse_quote! {
                    fn #new_method_ident(&self) -> impl std::fmt::Display;
                };
                
                trait_items.push((trait_item, new_method_sig));
            }
        }

        for item in &mut file.items {
            if let Item::Impl(impl_block) = item {
                impl_items.push(impl_block);
            }
        }

        for (trait_item, new_method_sig) in trait_items {
            trait_item.items.push(new_method_sig);

            for impl_block in &mut impl_items {
                if let Some((_, path, _)) = &impl_block.trait_ {
                    if path.segments.last().unwrap().ident == trait_item.ident {
                        let new_method_ident = Ident::new("new_method", Span::call_site());
                        let new_method_impl: syn::ImplItem = parse_quote! {
                            fn #new_method_ident(&self) -> impl std::fmt::Display {
                                "example"
                            }
                        };
                        impl_block.items.push(new_method_impl);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets traits and their implementations. It modifies a trait by adding a new method with an associated type or where clause not implemented in existing structs or enums. This new method requires a type or trait not currently in scope, like std::fmt::Display. The mutation introduces potential conflicts or missing trait implementations that could lead to compiler errors or unexpected behavior."
    }
}