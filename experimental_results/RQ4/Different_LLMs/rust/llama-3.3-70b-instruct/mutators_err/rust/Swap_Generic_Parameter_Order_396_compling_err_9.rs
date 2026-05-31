use syn::{File, Item, GenericParam, Generics, Data};
use crate::mutator::Mutator;

struct Swap_Generic_Parameter_Order_396;

impl Mutator for Swap_Generic_Parameter_Order_396 {
    fn name(&self) -> &str {
        "Swap_Generic_Parameter_Order_396"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Data::Struct(data) = &mut item_struct.data {
                    if let Some(generics) = &mut item_struct.generics {
                        let mut params: Vec<GenericParam> = generics.params.clone();
                        if params.len() > 1 {
                            params.swap(0, 1);
                            generics.params = params;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of generic parameters in struct definitions. This transformation tests the Rust compiler's handling of generic parameter order and default const expressions, which is relevant to the F-generic_const_exprs bug label."
    }
}