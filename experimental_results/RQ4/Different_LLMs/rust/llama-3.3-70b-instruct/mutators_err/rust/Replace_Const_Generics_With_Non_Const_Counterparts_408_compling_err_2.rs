impl Mutator for Replace_Const_Generics_With_Non_Const_Counterparts_408 {
    fn name(&self) -> &str {
        "Replace_Const_Generics_With_Non_Const_Counterparts_408"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_param = GenericParam::Type(syn::TypeParam {
                                attrs: param.attrs.clone(),
                                ident: param.ident.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                                default: None,
                                eq_token: None,
                            });
                            *param = new_param;
                        }
                    }
                }
            }

            if let syn::Item::Trait(trait_item) = item {
                if let Some(generics) = &mut trait_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_param = GenericParam::Type(syn::TypeParam {
                                attrs: param.attrs.clone(),
                                ident: param.ident.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                                default: None,
                                eq_token: None,
                            });
                            *param = new_param;
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_param = GenericParam::Type(syn::TypeParam {
                                attrs: param.attrs.clone(),
                                ident: param.ident.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                                default: None,
                                eq_token: None,
                            });
                            *param = new_param;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-const counterparts in structs, traits, and impls. This transformation aims to test the compiler's handling of generic parameters and their interaction with other language features, potentially uncovering bugs related to const generics and their evaluation."
    }
}