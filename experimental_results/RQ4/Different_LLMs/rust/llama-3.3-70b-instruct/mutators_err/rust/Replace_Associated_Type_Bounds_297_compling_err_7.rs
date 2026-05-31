use rand::Rng;
use rand::thread_rng;
use syn::{TraitBound, TraitBoundModifier, punctuated::Punctuated, PathSegment, PathArguments, TypeParamBound, Ident};
use proc_macro2::Span;
use crate::mutator::Mutator; // Import the Mutator trait

struct Replace_Associated_Type_Bounds_297;

impl Mutator for Replace_Associated_Type_Bounds_297 {
    fn name(&self) -> &str {
        "Replace_Associated_Type_Bounds_297"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for (i, item) in trait_item.items.iter_mut().enumerate() {
                    if let syn::TraitItem::Type(item_type) = item {
                        if let Some(bounds) = &mut item_type.bounds {
                            let mut rng = thread_rng();
                            let replace_bound = rng.gen_bool(0.5);
                            if replace_bound {
                                let new_bound = TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new(
                                                &format!("Bound_{}", i),
                                                Span::call_site(),
                                            ),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                };
                                *bounds = Punctuated::from_iter(vec![TypeParamBound::Trait(new_bound)]);
                            } else {
                                *bounds = Punctuated::new();
                            }
                        } else {
                            let mut rng = thread_rng();
                            let replace_bound = rng.gen_bool(0.5);
                            if replace_bound {
                                let new_bound = TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new(
                                                &format!("Bound_{}", i),
                                                Span::call_site(),
                                            ),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                };
                                item_type.bounds = Some(Punctuated::from_iter(vec![TypeParamBound::Trait(new_bound)]));
                            }
                        }
                    }
                }
            }
        }
    }
}