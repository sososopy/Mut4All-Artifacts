use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng};
use syn::{
    FnArg, Item, ItemFn, ReturnType, TraitBound, Type, TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated, token::{Plus},
    visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Alter_Combine_Traits_33;

impl Mutator for Alter_Combine_Traits_33 {
    fn name(&self) -> &str {
        "Alter_Combine_Traits_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                // Mutate function parameter traits
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            let bounds = &mut type_impl_trait.bounds;
                            let mut rng = thread_rng();
                            bounds.shuffle(&mut rng);
                            if !bounds.is_empty() {
                                bounds.pop();
                            }
                        }
                    }
                }
                // Mutate return type traits
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::TraitObject(type_trait_object) = &mut **ty {
                        let bounds = &mut type_trait_object.bounds;
                        let mut rng = thread_rng();
                        bounds.shuffle(&mut rng);
                        if !bounds.is_empty() {
                            bounds.pop();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator alters function signatures by shuffling and removing traits in `impl Trait` and `dyn Trait` expressions. It tests the Rust compiler's handling of trait bounds, especially focusing on trait combination and the new `dyn*` feature."
    }
}