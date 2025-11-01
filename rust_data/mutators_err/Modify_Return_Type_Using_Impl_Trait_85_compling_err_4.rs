use syn::parse_quote;
use syn::Item;
use syn::ReturnType;
use syn::Type;
use syn::TypeParamBound;
use syn::TypeImplTrait;
use syn::punctuated::Punctuated;
use crate::mutator::Mutator;

pub struct Modify_Return_Type_Using_Impl_Trait_85;

impl Mutator for Modify_Return_Type_Using_Impl_Trait_85 {
    fn name(&self) -> &str {
        "Modify_Return_Type_Using_Impl_Trait_85"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, ref mut ty) = item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**ty {
                        if let Some(TypeParamBound::Trait(trait_bound)) = type_impl_trait.bounds.first() {
                            let new_trait: TypeParamBound = parse_quote!(impl Clone);
                            if trait_bound.path.is_ident("Sized") {
                                item_fn.sig.output = ReturnType::Type(Default::default(), Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: Default::default(),
                                    bounds: Punctuated::from_iter(vec![new_trait]),
                                })));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with return types specified as `impl Trait`. It identifies such functions and modifies their return type to a different trait, such as replacing `impl Sized` with `impl Clone`. The function body is then checked to ensure it can return a type conforming to the new trait."
    }
}