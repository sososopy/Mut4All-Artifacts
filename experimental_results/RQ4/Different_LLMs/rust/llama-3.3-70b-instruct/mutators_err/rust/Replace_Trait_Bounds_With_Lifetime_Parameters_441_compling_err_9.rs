use crate::mutator::Mutator;
use syn::{TypeParamBound, TraitBound, TraitBoundModifier, BoundLifetimes, token, punctuated::Punctuated, GenericParam, LifetimeParam, Ident};
use proc_macro2::Span;

struct Replace_Trait_Bounds_With_Lifetime_Parameters_441;

impl Mutator for Replace_Trait_Bounds_With_Lifetime_Parameters_441 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_With_Lifetime_Parameters_441"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_bound in &mut trait_item.supertraits {
                    if let TypeParamBound::Trait(trait_bound) = trait_bound {
                        let new_trait_bound = TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: Some(BoundLifetimes {
                                for_token: token::For {
                                    span: Span::call_site(),
                                },
                                lt_token: token::Lt {
                                    span: Span::call_site(),
                                },
                                gt_token: token::Gt {
                                    span: Span::call_site(),
                                },
                                lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                                    attrs: vec![],
                                    lifetime: syn::Lifetime {
                                        apostrophe: Span::call_site(),
                                        ident: Ident::new("a", Span::call_site()),
                                    },
                                    bounds: Punctuated::new(),
                                    colon_token: None,
                                })]),
                            }),
                            path: trait_bound.path.clone(),
                        };
                        *trait_bound = TypeParamBound::Trait(new_trait_bound);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds with lifetime parameters. This transformation tests the compiler's ability to handle traits with lifetime parameters and can help expose bugs related to lifetime handling."
    }
}