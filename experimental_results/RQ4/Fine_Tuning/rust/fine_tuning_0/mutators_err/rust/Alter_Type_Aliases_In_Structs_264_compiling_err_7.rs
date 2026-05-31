use std::collections::HashMap;
use crate::mutator::Mutator;
use syn::{File, Item, Type};

pub struct Alter_Type_Aliases_In_Structs_264;

impl Mutator for Alter_Type_Aliases_In_Structs_264 {
    fn name(&self) -> &str {
        "Alter_Type_Aliases_In_Structs_264"
    }
    fn mutate(&self, file: &mut File) {
        let mut alias_map = HashMap::new();

        for item in &file.items {
            if let Item::Type(type_item) = item {
                alias_map.insert(type_item.ident.clone(), &type_item.ty);
            }
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Path(type_path) = &mut field.ty {
                        if let Some(last_segment) = type_path.path.segments.last() {
                            if let Some(expanded_type) = alias_map.get(&last_segment.ident) {
                                field.ty = *(**expanded_type).clone();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions that use type aliases for their fields. It replaces the type alias with the actual type it represents, ensuring the transformation is semantically equivalent. This aims to test the compiler's handling of type aliases, especially in the context of trait bounds and generics, potentially uncovering issues in type resolution."
    }
}