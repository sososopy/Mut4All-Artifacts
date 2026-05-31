use crate::mutator::Mutator;
use syn::{Item, GenericParam, Ident};
use proc_macro2::Ident;
use rand::thread_rng;
use rand::distributions::Alphanumeric;
use syn::Lifetime;

struct Modify_Lifetime_Parameters_442;

impl Mutator for Modify_Lifetime_Parameters_442 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Parameters_442"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if let Some(generics) = &mut trait_item.generics {
                    let generics_mut = generics;
                    for param in &mut generics_mut.params {
                        if let GenericParam::Lifetime(lifetime) = param {
                            let new_lifetime_name = Ident::new(&format!("'{}", thread_rng().sample(Alphanumeric)), lifetime.lifetime.span());
                            lifetime.lifetime = Lifetime {
                                apostrophe: lifetime.lifetime.apostrophe,
                                ident: new_lifetime_name,
                            };
                        }
                    }
                }
            } else if let Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    let generics_mut = generics;
                    for param in &mut generics_mut.params {
                        if let GenericParam::Lifetime(lifetime) = param {
                            let new_lifetime_name = Ident::new(&format!("'{}", thread_rng().sample(Alphanumeric)), lifetime.lifetime.span());
                            lifetime.lifetime = Lifetime {
                                apostrophe: lifetime.lifetime.apostrophe,
                                ident: new_lifetime_name,
                            };
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets Rust code that utilizes lifetime parameters, specifically in the context of trait definitions and implementations. It modifies these parameters by changing their names to different valid identifiers, removing or adding lifetime parameters, and replacing existing lifetime parameters with new ones."
    }
}