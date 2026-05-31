use syn::{parse_quote, GenericParam};
use rand::thread_rng;
use crate::mutator::Mutator;

struct Replace_Generic_Args_With_Constants_358;

impl Mutator for Replace_Generic_Args_With_Constants_358 {
    fn name(&self) -> &str {
        "Replace_Generic_Args_With_Constants_358"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                if let Some(generics) = &mut trait_def.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            let const_type = match rng.gen_range(0..3) {
                                0 => parse_quote!(i32),
                                1 => parse_quote!(String),
                                2 => parse_quote!(Vec<i32>),
                                _ => unreachable!(),
                            };
                            type_param.default = Some(const_type);
                        }
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            let const_type = match rng.gen_range(0..3) {
                                0 => parse_quote!(i32),
                                1 => parse_quote!(String),
                                2 => parse_quote!(Vec<i32>),
                                _ => unreachable!(),
                            };
                            type_param.default = Some(const_type);
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_def) = item {
                if let Some(generics) = &mut impl_def.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            let const_type = match rng.gen_range(0..3) {
                                0 => parse_quote!(i32),
                                1 => parse_quote!(String),
                                2 => parse_quote!(Vec<i32>),
                                _ => unreachable!(),
                            };
                            type_param.default = Some(const_type);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic arguments in trait definitions and function signatures with constants. This transformation helps trigger bugs related to generic const expressions and tests the compiler's ability to handle generic const expressions."
    }
}