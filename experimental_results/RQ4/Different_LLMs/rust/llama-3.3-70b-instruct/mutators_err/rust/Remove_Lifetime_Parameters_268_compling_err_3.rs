use syn::{File, GenericParam, Item, ItemEnum, ItemStruct};
use syn::generics::Generics;

struct Remove_Lifetime_Parameters_268;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Remove_Lifetime_Parameters_268 {
    fn name(&self) -> &str {
        "Remove_Lifetime_Parameters_268"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                if let Generics::None = enum_item.generics {
                    continue;
                }
                if let Some(generics) = &mut enum_item.generics {
                    if let syn::Generics::WhereClauses(_) = generics {
                        continue;
                    }
                    generics.params.retain(|param| !matches!(param, GenericParam::Lifetime(_)));
                }
            }
            if let Item::Struct(struct_item) = item {
                if let Generics::None = struct_item.generics {
                    continue;
                }
                if let Some(generics) = &mut struct_item.generics {
                    if let syn::Generics::WhereClauses(_) = generics {
                        continue;
                    }
                    generics.params.retain(|param| !matches!(param, GenericParam::Lifetime(_)));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes lifetime parameters from enum and struct definitions. This transformation can help trigger bugs related to lifetime management and trait implementation, by creating situations where the compiler must handle missing or incorrect lifetime information."
    }
}