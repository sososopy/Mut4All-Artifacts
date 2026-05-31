impl Mutator for Swap_Default_And_Non_Default_Generic_Params_348 {
    fn name(&self) -> &str {
        "Swap_Default_And_Non_Default_Generic_Params_348"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    self.mutate_generics(generics);
                }
            } else if let syn::Item::Enum(enum_item) = item {
                if let Some(generics) = &mut enum_item.generics {
                    self.mutate_generics(generics);
                }
            } else if let syn::Item::Trait(trait_item) = item {
                if let Some(generics) = &mut trait_item.generics {
                    self.mutate_generics(generics);
                }
            } else if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    self.mutate_generics(generics);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of a default and a non-default generic parameter in a generic definition. This transformation tests the compiler's handling of default and non-default generic parameters, especially when they are mixed together."
    }
}

impl Swap_Default_And_Non_Default_Generic_Params_348 {
    fn mutate_generics(&self, generics: &mut syn::Generics) {
        let mut default_params = Vec::new();
        let mut non_default_params = Vec::new();

        for param in &generics.params {
            if let GenericParam::Type(TypeParam { default, .. }) = param {
                if default.is_some() {
                    default_params.push(param);
                } else {
                    non_default_params.push(param);
                }
            }
        }

        if !default_params.is_empty() && !non_default_params.is_empty() {
            let default_param = default_params.choose(&mut thread_rng()).unwrap();
            let non_default_param = non_default_params.choose(&mut thread_rng()).unwrap();

            let default_param_index = generics.params.iter().position(|p| p == *default_param).unwrap();
            let non_default_param_index = generics.params.iter().position(|p| p == *non_default_param).unwrap();

            generics.params.as_mut().unwrap().swap(default_param_index, non_default_param_index);
        }
    }
}