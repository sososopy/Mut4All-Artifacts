use syn::{File, Item, GenericParam, Generics};
use crate::mutator::Mutator;

struct Replace_Type_Parameter_With_Unconstrained_Type_453;

impl Mutator for Replace_Type_Parameter_With_Unconstrained_Type_453 {
    fn name(&self) -> &str {
        "Replace_Type_Parameter_With_Unconstrained_Type_453"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    let generics_mut = generics;
                    for param in &mut generics_mut.params {
                        if let GenericParam::Type(type_param) = param {
                            if !type_param.bounds.is_empty() {
                                type_param.bounds.clear();
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constrained type parameters in trait implementations with unconstrained type parameters. This transformation tests the compiler's ability to handle and resolve type parameters correctly, especially in the context of trait specialization."
    }
}