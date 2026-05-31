impl Mutator for Replace_Const_Generic_With_Non_Const_Version_351 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Non_Const_Version_351"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.generics.params.is_empty() {
                    continue;
                }
                let generics = &mut func.sig.generics;
                for param in generics.type_params_mut() {
                    if let GenericParam::Const(param) = param {
                        let new_param = GenericParam::Type(syn::TypeParam {
                            attrs: param.attrs.clone(),
                            ident: param.ident.clone(),
                            colon_token: Some(param.colon_token.clone()),
                            bounds: Default::default(),
                            default: None,
                            eq_token: None,
                        });
                        *param = new_param;
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                if impl_item.generics.params.is_empty() {
                    continue;
                }
                let generics = &mut impl_item.generics;
                for param in generics.type_params_mut() {
                    if let GenericParam::Const(param) = param {
                        let new_param = GenericParam::Type(syn::TypeParam {
                            attrs: param.attrs.clone(),
                            ident: param.ident.clone(),
                            colon_token: Some(param.colon_token.clone()),
                            bounds: Default::default(),
                            default: None,
                            eq_token: None,
                        });
                        *param = new_param;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-const generic parameters. This transformation aims to test the compiler's handling of const generics and generic const expressions by replacing them with non-const versions, potentially triggering type resolution issues, const evaluation bugs, or unexpected behavior in generic function contexts."
    }
}