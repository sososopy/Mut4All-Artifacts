use crate::mutator::Mutator;
use syn::{Item, Type, TypePath};

pub struct Alter_Type_Aliases_In_Structs_102;

impl Mutator for Alter_Type_Aliases_In_Structs_102 {
    fn name(&self) -> &str {
        "Alter_Type_Aliases_In_Structs_102"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_alias_map = std::collections::HashMap::new();

        for item in &file.items {
            if let Item::Type(item_type) = item {
                let alias_ident = &item_type.ident;
                let alias_type = &item_type.ty;
                type_alias_map.insert(alias_ident.clone(), alias_type.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Path(TypePath { path, .. }) = &field.ty {
                        if let Some(last_segment) = path.segments.last() {
                            if let Some(expanded_type) = type_alias_map.get(&last_segment.ident) {
                                field.ty = expanded_type.clone();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions that utilize type aliases for their fields. It identifies type aliases and replaces them with their corresponding expanded type expressions. This can help in revealing issues related to type alias handling, especially in the context of trait bounds and generics."
    }
}