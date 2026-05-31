use crate::mutator::Mutator;
use syn::{Item, Fields};
use proc_macro2::{Ident, Span};

pub struct Rename_Unnamed_Fields_294;

impl Mutator for Rename_Unnamed_Fields_294 {
    fn name(&self) -> &str {
        "Rename_Unnamed_Fields_294"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    let mut field_index = 1;
                    if let Fields::Unnamed(fields_unnamed) = &mut item_struct.fields {
                        for field in &mut fields_unnamed.unnamed {
                            field.ident = Some(Ident::new(&format!("field{}", field_index), Span::call_site()));
                            field_index += 1;
                        }
                    }
                }
                Item::Union(item_union) => {
                    let mut field_index = 1;
                    for field in &mut item_union.fields.named {
                        if field.ident.is_none() {
                            field.ident = Some(Ident::new(&format!("field{}", field_index), Span::call_site()));
                            field_index += 1;
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets structs and unions with unnamed fields. The goal is to replace unnamed fields with named fields, maintaining the field type but providing a unique name. This mutation helps explore potential compiler bugs related to field uniqueness checks and name resolution by ensuring each unnamed field is given a unique name within the struct or union scope."
    }
}