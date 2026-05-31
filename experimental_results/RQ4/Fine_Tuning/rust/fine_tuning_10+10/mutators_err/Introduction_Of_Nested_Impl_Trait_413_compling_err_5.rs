use syn::parse_quote;
use syn::Item;
use syn::ReturnType;
use syn::Type;
use syn::TypeParamBound;
use syn::TypeImplTrait;
use syn::punctuated::Punctuated;
use crate::mutator::Mutator;

pub struct IntroductionOfNestedImplTrait413;

impl Mutator for IntroductionOfNestedImplTrait413 {
    fn name(&self) -> &str {
        "IntroductionOfNestedImplTrait413"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, ref mut ty) = item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**ty {
                        let original_trait_bounds = &type_impl_trait.bounds;
                        let new_trait_bound: TypeParamBound = parse_quote!(AnotherTrait<#original_trait_bounds>);
                        let new_type_impl_trait = TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(new_trait_bound);
                                bounds
                            },
                        };
                        *ty = Box::new(Type::ImplTrait(new_type_impl_trait));
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with `impl Trait` in their return type. It introduces a nested `impl Trait` to increase complexity. Specifically, it wraps the existing `impl Trait` with another `impl Trait` having a compatible trait bound, thereby testing the compiler's handling of nested trait structures."
    }
}