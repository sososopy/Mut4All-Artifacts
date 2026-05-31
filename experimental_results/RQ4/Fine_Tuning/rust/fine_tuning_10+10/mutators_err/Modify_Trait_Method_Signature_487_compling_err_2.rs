use syn::{
    ItemTrait, TraitItem, ImplItem, ItemImpl, TraitItemMethod, ImplItemMethod,
};

impl Mutator for Modify_Trait_Method_Signature_487 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Signature_487"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let TraitItem::Method(method) = trait_item {
                        if method.sig.asyncness.is_some() {
                            let new_generic_param: GenericParam = parse_quote!(T);
                            method.sig.generics.params.push(new_generic_param);
                        }
                    }
                }
            }
            
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Method(method) = impl_item {
                        if method.sig.asyncness.is_some() {
                            let new_generic_param: GenericParam = parse_quote!(T);
                            method.sig.generics.params.push(new_generic_param);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async methods within traits, adding an additional generic parameter `T` to their signatures. This transformation is propagated to the corresponding implementations, ensuring consistency. The mutation aims to test the compiler's handling of generic parameters in async contexts and the potential impacts on type inference and trait resolution."
    }
}