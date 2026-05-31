use proc_macro2::{Span, Ident};
use quote::quote;
use rand::{Rng, thread_rng};
use syn::{
    GenericParam, Lifetime, Item, LifetimeParam, parse_quote, File, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Add_Random_Lifetime_Bounds_137;

impl Mutator for Add_Random_Lifetime_Bounds_137 {
    fn name(&self) -> &str {
        "Add_Random_Lifetime_Bounds_137"
    }
    fn mutate(&self, file: &mut File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    let lifetime_ident = Ident::new(&format!("a{}", rng.gen::<u32>()), Span::call_site());
                    let lifetime = Lifetime::new(&format!("'{}", lifetime_ident), Span::call_site());
                    generics.params.push(parse_quote!(#lifetime));
                    for param in &mut generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            lifetime_param.bounds.push(lifetime.clone());
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            let lifetime_ident = Ident::new(&format!("a{}", rng.gen::<u32>()), Span::call_site());
                            let lifetime = Lifetime::new(&format!("'{}", lifetime_ident), Span::call_site());
                            generics.params.push(parse_quote!(#lifetime));
                            for param in &mut generics.params {
                                if let GenericParam::Lifetime(lifetime_param) = param {
                                    lifetime_param.bounds.push(lifetime.clone());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds random lifetime parameters with bounds to non-main function and impl signatures. This transformation introduces complex lifetime relationships, potentially leading to ICEs or inference failures in the compiler's lifetime resolution system. By generating random lifetime identifiers and bounds, it stresses the compiler's ability to handle intricate lifetime dependencies and constraints."
    }
}