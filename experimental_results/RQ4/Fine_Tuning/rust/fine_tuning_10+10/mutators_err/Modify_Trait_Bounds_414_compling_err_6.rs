use syn::parse_quote;
use crate::mutator::Mutator;
use syn::{Item, ReturnType, Type, TypeParamBound, token, punctuated::Punctuated};
use syn::token::Plus;
use proc_macro2::Span;

pub struct Modify_Trait_Bounds_414;

impl Mutator for Modify_Trait_Bounds_414 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_414"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut new_bounds: Punctuated<TypeParamBound, Plus> = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            new_bounds.push(bound.clone());
                        }
                        let new_type: Type = parse_quote! { impl for<'a> #new_bounds };
                        item_fn.sig.output = ReturnType::Type(
                            token::RArrow(Span::call_site()),
                            Box::new(new_type),
                        );
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies function return types that use `impl Trait` by introducing a generic `for<'a>` clause and adding complexity to the trait bounds. This is done by iterating over the function items in the AST, identifying those with `impl Trait` return types, and then expanding the trait bounds to include a new generic lifetime parameter. The mutation aims to test the compiler's handling of complex trait bounds and type inference."
    }
}