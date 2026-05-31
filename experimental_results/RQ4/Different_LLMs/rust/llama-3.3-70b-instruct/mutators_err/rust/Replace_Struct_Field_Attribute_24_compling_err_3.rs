use syn::{parse_quote, Item};
use crate::mutator::Mutator;

struct Replace_Struct_Field_Attribute_24;

impl Mutator for Replace_Struct_Field_Attribute_24 {
    fn name(&self) -> &str {
        "Replace_Struct_Field_Attribute_24"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Some(attrs) = &mut field.attrs {
                        let mut new_attrs = Vec::new();
                        for attr in attrs.iter() {
                            if attr.path.is_ident("should_panic") {
                                new_attrs.push(syn::Attribute {
                                    pound_token: attr.pound_token,
                                    style: attr.style,
                                    bracket_token: attr.bracket_token,
                                    path: parse_quote!(allow),
                                    meta: parse_quote!(dead_code),
                                });
                            } else if attr.path.is_ident("allow") {
                                new_attrs.push(syn::Attribute {
                                    pound_token: attr.pound_token,
                                    style: attr.style,
                                    bracket_token: attr.bracket_token,
                                    path: parse_quote!(doc),
                                    meta: parse_quote!(hidden),
                                });
                            } else {
                                new_attrs.push(attr.clone());
                            }
                        }
                        *attrs = new_attrs;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the attribute of a struct field with another attribute, such as replacing #[should_panic::skip] with #[allow(dead_code)] or #[doc(hidden)], to trigger potential bugs in the compiler's attribute handling code."
    }
}