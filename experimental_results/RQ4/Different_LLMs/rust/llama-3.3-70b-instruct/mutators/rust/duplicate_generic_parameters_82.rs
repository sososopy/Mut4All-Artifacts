use syn::{GenericParam, TypeParam, Ident};
use syn::token::Eq;
use crate::mutator::Mutator;

struct Duplicate_Generic_Parameters_82;

impl Mutator for Duplicate_Generic_Parameters_82 {
    fn name(&self) -> &str {
        "Duplicate_Generic_Parameters_82"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if struct_item.generics.params.is_empty() {
                    continue;
                }
                let mut new_params = struct_item.generics.params.clone();
                for param in &struct_item.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let new_param = GenericParam::Type(TypeParam {
                            attrs: type_param.attrs.clone(),
                            default: None,
                            eq_token: Some(Eq::default()),
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
                struct_item.generics.params = new_params;
            } else if let syn::Item::Enum(enum_item) = item {
                if enum_item.generics.params.is_empty() {
                    continue;
                }
                let mut new_params = enum_item.generics.params.clone();
                for param in &enum_item.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let new_param = GenericParam::Type(TypeParam {
                            attrs: type_param.attrs.clone(),
                            default: None,
                            eq_token: Some(Eq::default()),
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
                enum_item.generics.params = new_params;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates generic parameters in struct and enum definitions, creating new parameters with the same type and bounds but different names. This transformation tests the handling of repeated generic parameters and may lead to errors in type checking or inference."
    }
}