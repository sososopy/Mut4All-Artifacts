use syn::{GenericParam, Lifetime, LifetimeParam};
use proc_macro2::Span;

struct Replace_Generic_Lifetime_With_Concrete_Lifetime_382;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Replace_Generic_Lifetime_With_Concrete_Lifetime_382 {
    fn name(&self) -> &str {
        "Replace_Generic_Lifetime_With_Concrete_Lifetime_382"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    for param in generics.params_mut() {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime_param = LifetimeParam {
                                attrs: lifetime_param.attrs.clone(),
                                lifetime: Lifetime::new("'static", Span::call_site()),
                                colon_token: lifetime_param.colon_token.clone(),
                                bounds: lifetime_param.bounds.clone(),
                            };
                            *param = GenericParam::Lifetime(new_lifetime_param);
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in generics.params_mut() {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime_param = LifetimeParam {
                                attrs: lifetime_param.attrs.clone(),
                                lifetime: Lifetime::new("'static", Span::call_site()),
                                colon_token: lifetime_param.colon_token.clone(),
                                bounds: lifetime_param.bounds.clone(),
                            };
                            *param = GenericParam::Lifetime(new_lifetime_param);
                        }
                    }
                }
            }
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in generics.params_mut() {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime_param = LifetimeParam {
                                attrs: lifetime_param.attrs.clone(),
                                lifetime: Lifetime::new("'static", Span::call_site()),
                                colon_token: lifetime_param.colon_token.clone(),
                                bounds: lifetime_param.bounds.clone(),
                            };
                            *param = GenericParam::Lifetime(new_lifetime_param);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic lifetime parameters in structs, impls, and functions with concrete lifetime parameters, such as 'static. This transformation tests the compiler's handling of lifetimes and generic const expressions, potentially exposing bugs in lifetime handling and const evaluation."
    }
}