use syn::parse_quote;
use syn::Item;
use syn::ReturnType;
use syn::Type;
use syn::punctuated::Punctuated;
use syn::TypeParamBound;
use syn::TraitBound;
use crate::mutator::Mutator;

pub struct FunctionReturnTypeVariation189;

impl Mutator for FunctionReturnTypeVariation189 {
    fn name(&self) -> &str {
        "FunctionReturnTypeVariation189"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, ty) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **ty {
                        let mut new_bounds = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let new_trait_bound: TraitBound = parse_quote!(Future<Output = i32>);
                                new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            }
                        }
                        type_impl_trait.bounds = new_bounds;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function return types that use `impl Trait`. It changes the return type to a different trait, potentially causing type mismatches or incorrect trait bounds. This can expose bugs by forcing the compiler to handle unexpected trait requirements."
    }
}