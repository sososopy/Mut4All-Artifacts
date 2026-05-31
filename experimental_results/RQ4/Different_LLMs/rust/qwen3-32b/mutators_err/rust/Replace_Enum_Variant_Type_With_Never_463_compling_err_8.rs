pub struct Replace_Enum_Variant_Type_With_Never_463;

impl Mutator for Replace_Enum_Variant_Type_With_Never_463 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_Type_With_Never_463"
    }
    fn mutate(&self, file: &mut syn::File) {
        let feature_attr = parse_quote! { #![feature(never_type)] };
        file.attrs.push(feature_attr);

        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    match variant.fields {
                        syn::Fields::Unit => {
                            let mut fields_unnamed = syn::FieldsUnnamed {
                                paren_token: Default::default(),
                                unnamed: syn::punctuated::Punctuated::new(),
                            };
                            let field = syn::Field {
                                attrs: Vec::new(),
                                vis: parse_quote! { pub },
                                ident: None,
                                colon_token: None,
                                mutability: FieldMutability::None,
                                ty: parse_quote! { ! },
                            };
                            fields_unnamed.unnamed.push_value(field);
                            variant.fields = syn::Fields::Unnamed(fields_unnamed);
                        },
                        syn::Fields::Unnamed(ref mut fields_unnamed) => {
                            for field in &mut fields_unnamed.unnamed {
                                if let syn::Type::Never(_) = *field.ty {
                                    continue;
                                }
                                field.ty = parse_quote! { ! };
                            }
                        },
                        syn::Fields::Named(ref mut named) => {
                            for field in &mut named.named {
                                if let syn::Type::Never(_) = *field.ty {
                                    continue;
                                }
                                field.ty = parse_quote! { ! };
                            }
                        },
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}