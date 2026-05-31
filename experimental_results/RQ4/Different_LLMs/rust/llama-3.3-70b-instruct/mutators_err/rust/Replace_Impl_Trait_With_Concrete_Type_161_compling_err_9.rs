use syn::{parse_quote, Item, ReturnType, Type};
use crate::mutator::Mutator;

struct Replace_Impl_Trait_With_Concrete_Type_161;

impl Mutator for Replace_Impl_Trait_With_Concrete_Type_161 {
    fn name(&self) -> &str {
        "Replace_Impl_Trait_With_Concrete_Type_161"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = return_type {
                        // Replace impl Trait with a concrete type
                        let concrete_type = parse_quote!(Box<dyn std::fmt::Debug>);
                        *return_type = Box::new(concrete_type);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces `impl Trait` with a concrete type in function return types. This transformation changes the way the compiler handles trait objects and their lifetimes, potentially exposing bugs related to the `A-impl-trait` label."
    }
}