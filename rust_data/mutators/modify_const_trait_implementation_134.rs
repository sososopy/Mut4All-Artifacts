use crate::mutator::Mutator;
use syn::{parse_quote, Item, GenericParam, TypeParamBound, TraitBoundModifier, Attribute};

pub struct Modify_Const_Trait_Implementation_134;

impl Mutator for Modify_Const_Trait_Implementation_134 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Implementation_134"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if !trait_item.attrs.iter().any(|attr| attr.path().is_ident("const_trait")) {
                    trait_item.attrs.push(parse_quote!(#[const_trait]));
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                let has_const_trait_bound = impl_item.generics.params.iter().any(|param| {
                    if let GenericParam::Type(type_param) = param {
                        type_param.bounds.iter().any(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.modifier == TraitBoundModifier::None
                            } else {
                                false
                            }
                        })
                    } else {
                        false
                    }
                });

                if has_const_trait_bound {
                    let has_non_const_method = impl_item.items.iter().any(|impl_item| {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            method.sig.constness.is_none()
                        } else {
                            false
                        }
                    });

                    if !has_non_const_method {
                        if let Some(syn::ImplItem::Fn(method)) = impl_item.items.first_mut() {
                            method.sig.constness = None;
                        }
                    }

                    let new_method: syn::ImplItemFn = parse_quote! {
                        fn new_method<A: ~const MyTrait>(&self) -> i32 {
                            42
                        }
                    };

                    impl_item.items.push(syn::ImplItem::Fn(new_method));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies traits and their implementations to explore the const trait feature. It adds #[const_trait] to traits if missing, ensures at least one method in a const impl does not use ~const, and introduces a new method using ~const."
    }
}