use crate::mutator::Mutator;
use rand::Rng;
use rand::thread_rng;
use syn::{Item, File, Generics, GenericParam, TypeParam};

struct Replace_Generic_Type_Parameters_403;

impl Mutator for Replace_Generic_Type_Parameters_403 {
    fn name(&self) -> &str {
        "Replace_Generic_Type_Parameters_403"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let Generics::None = &struct_item.generics {
                    continue;
                }
                let params: Vec<_> = struct_item.generics.params().iter().collect();
                if params.len() > 1 {
                    let mut rng = thread_rng();
                    let idx1 = rng.gen_range(0..params.len());
                    let idx2 = rng.gen_range(0..params.len());
                    if idx1 != idx2 {
                        let param1 = params[idx1].clone();
                        let param2 = params[idx2].clone();
                        for param in struct_item.generics.params_mut() {
                            if let GenericParam::Type(TypeParam { ident, .. }) = param {
                                if ident == param1.ident {
                                    *param = param2.clone();
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Fn(func) = item {
                if let Generics::None = &func.sig.generics {
                    continue;
                }
                let params: Vec<_> = func.sig.generics.params().iter().collect();
                if params.len() > 1 {
                    let mut rng = thread_rng();
                    let idx1 = rng.gen_range(0..params.len());
                    let idx2 = rng.gen_range(0..params.len());
                    if idx1 != idx2 {
                        let param1 = params[idx1].clone();
                        let param2 = params[idx2].clone();
                        for param in func.sig.generics.params_mut() {
                            if let GenericParam::Type(TypeParam { ident, .. }) = param {
                                if ident == param1.ident {
                                    *param = param2.clone();
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                if let Generics::None = &impl_item.generics {
                    continue;
                }
                let params: Vec<_> = impl_item.generics.params().iter().collect();
                if params.len() > 1 {
                    let mut rng = thread_rng();
                    let idx1 = rng.gen_range(0..params.len());
                    let idx2 = rng.gen_range(0..params.len());
                    if idx1 != idx2 {
                        let param1 = params[idx1].clone();
                        let param2 = params[idx2].clone();
                        for param in impl_item.generics.params_mut() {
                            if let GenericParam::Type(TypeParam { ident, .. }) = param {
                                if ident == param1.ident {
                                    *param = param2.clone();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a type parameter in a generic type definition with another existing type parameter in the same definition. This transformation aims to test the compiler's ability to handle complex generic types and identify potential issues in its type checking and instantiation mechanisms."
    }
}