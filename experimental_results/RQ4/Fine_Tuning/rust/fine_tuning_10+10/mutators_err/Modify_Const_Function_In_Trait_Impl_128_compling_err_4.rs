use syn::{ImplItem, Item, TraitItem};

pub struct Modify_Const_Function_In_Trait_Impl_128;

impl crate::mutator::Mutator for Modify_Const_Function_In_Trait_Impl_128 {
    fn name(&self) -> &str {
        "Modify_Const_Function_In_Trait_Impl_128"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if method.sig.constness.is_some() {
                            method.sig.constness = None;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets const functions within trait implementations. It identifies const functions and removes the `const` keyword, converting them into non-const functions while preserving the original functionality."
    }
}