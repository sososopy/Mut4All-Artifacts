use rand::thread_rng;
use syn::{Item, GenericParam, LifetimeParam, Type};

struct Replace_Lifetime_Parameter_97;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Replace_Lifetime_Parameter_97 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameter_97"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_def) = item {
                if let Some(generics) = &mut struct_def.generics {
                    let mut lifetimes: Vec<LifetimeParam> = generics
                        .params
                        .iter()
                        .filter_map(|param| match param {
                            GenericParam::Lifetime(lifetime) => Some(lifetime.clone()),
                            _ => None,
                        })
                        .collect();

                    if lifetimes.len() > 1 {
                        let mut rng = thread_rng();
                        let index_to_replace = rng.gen_range(0..lifetimes.len());
                        let replacement_lifetime = lifetimes
                            .iter()
                            .filter(|lifetime| lifetime.lifetime != lifetimes[index_to_replace].lifetime)
                            .choose(&mut rng)
                            .unwrap()
                            .clone();

                        for param in &mut generics.params {
                            if let GenericParam::Lifetime(lifetime) = param {
                                if lifetime.lifetime == lifetimes[index_to_replace].lifetime {
                                    lifetime.lifetime = replacement_lifetime.lifetime.clone();
                                }
                            }
                        }

                        match &mut struct_def.fields {
                            syn::Fields::Named(fields_named) => {
                                for field_named in &mut fields_named.named {
                                    if let Type::Reference(type_ref) = &mut field_named.ty {
                                        if let Some(lifetime) = &type_ref.lifetime {
                                            if *lifetime == lifetimes[index_to_replace].lifetime {
                                                type_ref.lifetime = Some(replacement_lifetime.lifetime.clone());
                                            }
                                        }
                                    }
                                }
                            }
                            syn::Fields::Unnamed(fields_unnamed) => {
                                for field_unnamed in &mut fields_unnamed.unnamed {
                                    if let Type::Reference(type_ref) = &mut field_unnamed.ty {
                                        if let Some(lifetime) = &type_ref.lifetime {
                                            if *lifetime == lifetimes[index_to_replace].lifetime {
                                                type_ref.lifetime = Some(replacement_lifetime.lifetime.clone());
                                            }
                                        }
                                    }
                                }
                            }
                            syn::Fields::Unit => {}
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a lifetime parameter in a struct definition with another existing lifetime parameter in the same scope. This transformation tests the compiler's handling of lifetime parameters and their substitution in generic types."
    }
}