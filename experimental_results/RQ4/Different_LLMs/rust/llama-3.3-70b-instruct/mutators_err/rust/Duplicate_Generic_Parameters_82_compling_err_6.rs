impl Mutator for Duplicate_Generic_Parameters_82 {
    fn name(&self) -> &str {
        "Duplicate_Generic_Parameters_82"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let syn::Generics::None = struct_item.generics {
                    continue;
                }
                let generics = struct_item.generics.take().unwrap();
                let mut new_params = generics.params.clone();
                for param in &generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let new_param = GenericParam::Type(TypeParam {
                            attrs: type_param.attrs.clone(),
                            default: None,
                            eq_token: Some(token::Eq::default()),
                            ident: Ident::new(
                                &format!("{}_2", type_param.ident),
                                type_param.ident.span(),
                            ),
                            colon_token: type_param.colon_token,
                            bounds: type_param.bounds.clone(),
                        });
                        new_params.push(new_param);
                    }
                }
                struct_item.generics = syn::Generics {
                    params: new_params,
                    where_clause: generics.where_clause,
                    gt_token: generics.gt_token,
                };
            } else if let syn::Item::Enum(enum_item) = item {
                if let syn::Generics::None = enum_item.generics {
                    continue;
                }
                let generics = enum_item.generics.take().unwrap();
                let mut new_params = generics.params.clone();
                for param in &generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let new_param = GenericParam::Type(TypeParam {
                            attrs: type_param.attrs.clone(),
                            default: None,
                            eq_token: Some(token::Eq::default()),
                            ident: Ident::new(
                                &format!("{}_2", type_param.ident),
                                type_param.ident.span(),
                            ),
                            colon_token: type_param.colon_token,
                            bounds: type_param.bounds.clone(),
                        });
                        new_params.push(new_param);
                    }
                }
                enum_item.generics = syn::Generics {
                    params: new_params,
                    where_clause: generics.where_clause,
                    gt_token: generics.gt_token,
                };
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates generic parameters in struct and enum definitions, creating new parameters with the same type and bounds but different names. This transformation tests the handling of repeated generic parameters and may lead to errors in type checking or inference."
    }
}