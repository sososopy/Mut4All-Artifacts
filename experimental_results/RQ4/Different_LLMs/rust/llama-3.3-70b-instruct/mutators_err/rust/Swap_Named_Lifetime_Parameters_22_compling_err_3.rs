use crate::mutator::Mutator;
use syn::{LifetimeParam, GenericParam};
use rand::thread_rng;

struct Swap_Named_Lifetime_Parameters_22;

impl Mutator for Swap_Named_Lifetime_Parameters_22 {
    fn name(&self) -> &str {
        "Swap_Named_Lifetime_Parameters_22"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut lifetime_params: Vec<syn::LifetimeParam> = func.sig.generics.lifetimes().clone().into_iter().collect();
                if lifetime_params.len() > 1 {
                    let mut rng = thread_rng();
                    let idx1 = rng.gen_range(0..lifetime_params.len());
                    let idx2 = rng.gen_range(0..lifetime_params.len());
                    lifetime_params.swap(idx1, idx2);
                    func.sig.generics.params.push(syn::GenericParam::Lifetime(syn::LifetimeParam {
                        attrs: Default::default(),
                        lifetimes: lifetime_params,
                    }));
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut lifetime_params: Vec<syn::LifetimeParam> = func.sig.generics.lifetimes().clone().into_iter().collect();
                        if lifetime_params.len() > 1 {
                            let mut rng = thread_rng();
                            let idx1 = rng.gen_range(0..lifetime_params.len());
                            let idx2 = rng.gen_range(0..lifetime_params.len());
                            lifetime_params.swap(idx1, idx2);
                            func.sig.generics.params.push(syn::GenericParam::Lifetime(syn::LifetimeParam {
                                attrs: Default::default(),
                                lifetimes: lifetime_params,
                            }));
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the positions of two randomly chosen named lifetime parameters in function signatures. This transformation tests the compiler's ability to correctly infer and check lifetimes under altered parameter orders, potentially exposing bugs in the handling of named lifetimes, especially in async/await contexts."
    }
}