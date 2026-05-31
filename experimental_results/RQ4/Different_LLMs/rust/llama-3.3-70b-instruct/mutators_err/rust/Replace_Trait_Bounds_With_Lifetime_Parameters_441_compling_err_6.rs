use crate::mutator::Mutator;
use syn::{TypeParamBound, TraitBound, TraitBoundModifier, BoundLifetimes, token, punctuated::Punctuated, GenericParam, LifetimeParam, Ident, Span, Lifetime};

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
                                    spans: Default::default(),
                                },
                                lt_token: token::Lt {
                                    spans: Default::default(),
                                },
                                gt_token: token::Gt {
                                    spans: Default::default(),
                                },
                                lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                                    attrs: vec![],
                                    lifetime: Lifetime {
                                        apostrophe: syn::Lifetime {
                                            spans: Default::default(),
                                        },
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