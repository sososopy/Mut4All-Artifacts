use syn::{Item, GenericParam, Lifetime, Ident, LifetimeParam};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Change_Lifetime_Parameters_30;

impl Mutator for Change_Lifetime_Parameters_30 {
    fn name(&self) -> &str {
        "Change_Lifetime_Parameters_30"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in generics.params_mut() {
                        if let GenericParam::Lifetime(lifetime) = param {
                            let new_lifetime = if lifetime.lifetime.ident == "a" {
                                Lifetime {
                                    apostrophe: lifetime.lifetime.apostrophe,
                                    ident: Ident::new("static", Span::call_site()),
                                }
                            } else {
                                Lifetime {
                                    apostrophe: lifetime.lifetime.apostrophe,
                                    ident: Ident::new("a", Span::call_site()),
                                }
                            };
                            *param = GenericParam::Lifetime(LifetimeParam {
                                attrs: lifetime.attrs.clone(),
                                lifetime: new_lifetime,
                                bounds: Default::default(),
                                colon_token: Default::default(),
                            });
                        }
                    }
                }
            } else if let Item::Struct(struct_def) = item {
                if let Some(generics) = &mut struct_def.generics {
                    for param in generics.params_mut() {
                        if let GenericParam::Lifetime(lifetime) = param {
                            let new_lifetime = if lifetime.lifetime.ident == "a" {
                                Lifetime {
                                    apostrophe: lifetime.lifetime.apostrophe,
                                    ident: Ident::new("static", Span::call_site()),
                                }
                            } else {
                                Lifetime {
                                    apostrophe: lifetime.lifetime.apostrophe,
                                    ident: Ident::new("a", Span::call_site()),
                                }
                            };
                            *param = GenericParam::Lifetime(LifetimeParam {
                                attrs: lifetime.attrs.clone(),
                                lifetime: new_lifetime,
                                bounds: Default::default(),
                                colon_token: Default::default(),
                            });
                        }
                    }
                }
            } else if let Item::Impl(impl_def) = item {
                if let Some(generics) = &mut impl_def.generics {
                    for param in generics.params_mut() {
                        if let GenericParam::Lifetime(lifetime) = param {
                            let new_lifetime = if lifetime.lifetime.ident == "a" {
                                Lifetime {
                                    apostrophe: lifetime.lifetime.apostrophe,
                                    ident: Ident::new("static", Span::call_site()),
                                }
                            } else {
                                Lifetime {
                                    apostrophe: lifetime.lifetime.apostrophe,
                                    ident: Ident::new("a", Span::call_site()),
                                }
                            };
                            *param = GenericParam::Lifetime(LifetimeParam {
                                attrs: lifetime.attrs.clone(),
                                lifetime: new_lifetime,
                                bounds: Default::default(),
                                colon_token: Default::default(),
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The Change_Lifetime_Parameters_30 mutation operator targets lifetime parameters in Rust code and changes them to 'static or another existing lifetime parameter. This transformation tests the compiler's handling of lifetime parameters and can potentially introduce bugs if the new lifetime parameter is not compatible with the rest of the code."
    }
}