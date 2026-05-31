impl Mutator for Async_Trait_Removal_492 {
    fn name(&self) -> &str {
        "Async_Trait_Removal_492"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if let Some(attr) = trait_item.attrs.iter().find(|attr| attr.path().is_ident("async_trait")) {
                    trait_item.attrs.retain(|attr| attr.path().is_ident("async_trait") == false);
                    for item in &mut trait_item.items {
                        if let syn::TraitItem::Method(method) = item {
                            if let ReturnType::Type(_, return_type) = &method.sig.output {
                                if let Type::ImplTrait(_) = *return_type {
                                    method.sig.output = ReturnType::Default;
                                }
                            }
                            if method.sig.ident == "async" {
                                method.sig.ident = Ident::new("sync", Span::call_site());
                            }
                        } else if let syn::TraitItem::Method(method) = item {
                            if let ReturnType::Type(_, return_type) = &method.sig.output {
                                if let Type::ImplTrait(_) = *return_type {
                                    method.sig.output = ReturnType::Default;
                                }
                            }
                            if method.sig.ident == "async" {
                                method.sig.ident = Ident::new("sync", Span::call_site());
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes the async_trait attribute from trait implementations, effectively changing async functions to regular functions. This transformation tests the compiler's handling of trait implementations with asynchronous functions, particularly in the context of the async_trait macro and the F-unboxed_closures feature."
    }
}