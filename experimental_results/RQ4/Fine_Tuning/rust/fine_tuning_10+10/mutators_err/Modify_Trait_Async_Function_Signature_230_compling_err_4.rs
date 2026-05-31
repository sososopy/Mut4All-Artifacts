use syn::{ItemTrait, TraitItem, Item, ReturnType, parse_quote, TraitItemMethod};
use crate::mutator::Mutator;

pub struct Modify_Trait_Async_Function_Signature_230;

impl Mutator for Modify_Trait_Async_Function_Signature_230 {
    fn name(&self) -> &str {
        "Modify_Trait_Async_Function_Signature_230"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let TraitItem::Method(method) = item {
                        if method.sig.asyncness.is_some() {
                            if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                                *ty = Box::new(parse_quote! { impl Future<Output = #ty> });
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies async functions in traits and modifies their return type to use `impl Future<Output = Self::Return>`. This stresses the compiler's type inference and trait resolution mechanisms, especially with associated type bounds and unboxed closures."
    }
}