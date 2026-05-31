use crate::mutator::Mutator;

pub struct Modify_Trait_Method_Signature_147;

impl Mutator for Modify_Trait_Method_Signature_147 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Signature_147"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(ref mut trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(ref mut method) = trait_item {
                        if method.sig.constness.is_some() {
                            for param in &mut method.sig.generics.params {
                                if let GenericParam::Const(ref mut const_param) = param {
                                    const_param.ty = parse_quote! { &str };
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies trait methods with const generic parameters and alters the default type of these parameters to an incompatible type, such as changing 'usize' to '&str'. This is intended to create type conflicts, testing the compiler's ability to handle such errors in trait method signatures."
    }
}