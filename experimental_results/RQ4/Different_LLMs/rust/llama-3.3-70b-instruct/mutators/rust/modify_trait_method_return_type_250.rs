use syn::{Ident, TypeImplTrait, TypeParamBound, Type};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Modify_Trait_Method_Return_Type_250;

impl Mutator for Modify_Trait_Method_Return_Type_250 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Return_Type_250"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, ty) = &mut func.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**ty {
                                let mut new_bounds = type_impl_trait.bounds.clone();
                                for bound in &mut new_bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.segments.last().unwrap().ident == "FnOnce" {
                                            trait_bound.path.segments.last_mut().unwrap().ident = Ident::new("FnMut", Span::call_site());
                                        } else if trait_bound.path.segments.last().unwrap().ident == "FnMut" {
                                            trait_bound.path.segments.last_mut().unwrap().ident = Ident::new("Fn", Span::call_site());
                                        }
                                    }
                                }
                                *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: type_impl_trait.impl_token,
                                    bounds: new_bounds,
                                }));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait method return types involving closures or higher-order functions. It modifies the return type to introduce potential errors related to trait method resolution, type inference, or closure handling."
    }
}