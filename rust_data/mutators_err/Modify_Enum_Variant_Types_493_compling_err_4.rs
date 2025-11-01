use crate::mutator::Mutator;
use syn::{Item, Type};
use rand::thread_rng;
use rand::prelude::{IteratorRandom, SliceRandom};

pub struct Modify_Enum_Variant_Types_493;

impl Mutator for Modify_Enum_Variant_Types_493 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_Types_493"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if item_enum.variants.len() < 2 {
                    continue;
                }
                let mut rng = thread_rng();
                let variants = &mut item_enum.variants;
                let mut types: Vec<&Type> = variants.iter()
                    .filter_map(|v| match &v.fields {
                        syn::Fields::Unnamed(fields) if fields.unnamed.len() == 1 => {
                            Some(&fields.unnamed.first().unwrap().ty)
                        }
                        _ => None,
                    })
                    .collect();

                if types.len() < 2 {
                    continue;
                }

                let new_type = types.choose(&mut rng).unwrap().clone();
                let variant_to_modify = variants.iter_mut()
                    .filter_map(|v| match &mut v.fields {
                        syn::Fields::Unnamed(fields) if fields.unnamed.len() == 1 => {
                            Some((v, &mut fields.unnamed.first_mut().unwrap().ty))
                        }
                        _ => None,
                    })
                    .choose(&mut rng);

                if let Some((variant, ty)) = variant_to_modify {
                    *ty = new_type.clone();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with multiple variants, each associated with different data types. It randomly selects a variant and changes its associated data type to another type used within the same enum. This transformation aims to expose potential compiler bugs by introducing type mismatches and testing the compiler's handling of type resolution and variant management, especially in complex scenarios involving async functions and traits."
    }
}