use crate::mutator::Mutator;
use syn::visit_mut::VisitMut;
use syn::{TraitBound, GenericParam, Lifetime, LifetimeParam, BoundLifetimes};
use syn::punctuated::Punctuated;
use syn::token;
use proc_macro2::Span;

pub struct Modify_Trait_Bounds_408;

impl Mutator for Modify_Trait_Bounds_408 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_408"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TraitBoundModifierVisitor;

        impl VisitMut for TraitBoundModifierVisitor {
            fn visit_trait_bound_mut(&mut self, i: &mut TraitBound) {
                if let Some(lifetimes) = &mut i.lifetimes {
                    for lifetime_param in &mut lifetimes.lifetimes {
                        if let GenericParam::Lifetime(lifetime_param) = lifetime_param {
                            lifetime_param.lifetime = Lifetime::new("'a", Span::call_site());
                        }
                    }
                } else {
                    let new_lifetime = Lifetime::new("'a", Span::call_site());
                    let lifetime_param = LifetimeParam::new(new_lifetime.clone());
                    let bound_lifetimes = BoundLifetimes {
                        for_token: token::For(Span::call_site()),
                        lt_token: token::Lt(Span::call_site()),
                        lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(lifetime_param)]),
                        gt_token: token::Gt(Span::call_site()),
                    };
                    i.lifetimes = Some(bound_lifetimes);
                }
                syn::visit_mut::visit_trait_bound_mut(self, i);
            }
        }

        let mut visitor = TraitBoundModifierVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets traits with higher-ranked trait bounds (HRTBs). It modifies these bounds by replacing non-lifetime variables with a specific lifetime, such as 'a. This change forces the compiler to handle lifetimes explicitly, potentially revealing bugs related to lifetime management and variance. By applying this mutation, we can explore issues around lifetime handling in the compiler."
    }
}