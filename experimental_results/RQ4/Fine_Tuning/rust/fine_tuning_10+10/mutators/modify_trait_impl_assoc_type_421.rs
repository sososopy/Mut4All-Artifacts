use crate::mutator::Mutator;
use syn::{visit::Visit, Item, ItemImpl, Type};
use rand::thread_rng;
use rand::seq::SliceRandom;

pub struct Modify_Trait_Impl_Assoc_Type_421;

impl Mutator for Modify_Trait_Impl_Assoc_Type_421 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_Assoc_Type_421"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut type_candidates = Vec::new();
        
        struct TypeCollector<'a> {
            types: &'a mut Vec<Type>,
        }

        impl<'a> Visit<'_> for TypeCollector<'a> {
            fn visit_type(&mut self, node: &Type) {
                self.types.push(node.clone());
                syn::visit::visit_type(self, node);
            }
        }

        let mut collector = TypeCollector {
            types: &mut type_candidates,
        };
        collector.visit_file(file);

        for item in &mut file.items {
            if let Item::Impl(ItemImpl { trait_, items, .. }) = item {
                if let Some((_, path, _)) = trait_ {
                    for impl_item in items {
                        if let syn::ImplItem::Type(impl_type) = impl_item {
                            if impl_type.ident == "Output" {
                                if let Some(new_type) = type_candidates.choose(&mut thread_rng()) {
                                    impl_type.ty = new_type.clone();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait implementations with associated types. It identifies such implementations and changes the associated type to another type found within the seed program. This is done to explore potential inconsistencies in trait coherence and specialization."
    }
}