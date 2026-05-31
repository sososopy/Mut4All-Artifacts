use syn::{File, Generics, GenericParam, Item, ItemStruct, ItemEnum};
use crate::mutator::Mutator;

struct Swap_Const_Generics_And_Type_Generics_108;

impl Mutator for Swap_Const_Generics_And_Type_Generics_108 {
    fn name(&self) -> &str {
        "Swap_Const_Generics_And_Type_Generics_108"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if struct_item.generics.params.is_empty() {
                    continue;
                }
                let mut const_generics = Vec::new();
                let mut type_generics = Vec::new();
                for param in &mut struct_item.generics.params {
                    match param {
                        GenericParam::Const(_) => const_generics.push(param.clone()),
                        GenericParam::Type(_) => type_generics.push(param.clone()),
                        _ => {}
                    }
                }
                struct_item.generics.params.clear();
                struct_item.generics.params.extend(type_generics);
                struct_item.generics.params.extend(const_generics);
            }
            if let Item::Enum(enum_item) = item {
                if enum_item.generics.params.is_empty() {
                    continue;
                }
                let mut const_generics = Vec::new();
                let mut type_generics = Vec::new();
                for param in &mut enum_item.generics.params {
                    match param {
                        GenericParam::Const(_) => const_generics.push(param.clone()),
                        GenericParam::Type(_) => type_generics.push(param.clone()),
                        _ => {}
                    }
                }
                enum_item.generics.params.clear();
                enum_item.generics.params.extend(type_generics);
                enum_item.generics.params.extend(const_generics);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of const generic parameters and type generic parameters in a generic struct or enum definition. This transformation tests the compiler's ability to handle the scoping and evaluation of const expressions in the context of generic definitions."
    }
}