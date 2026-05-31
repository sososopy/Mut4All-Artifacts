use crate::mutator::Mutator;
use syn::{GenericParam, TypeParamBound, TraitBound, token, punctuated::Punctuated};
use proc_macro2::Span;

struct Change_Trait_Bound_494;

impl Mutator for Change_Trait_Bound_494 {
    fn name(&self) -> &str {
        "Change_Trait_Bound_494"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = func.sig.generics.clone();
                        for param in &mut generics.params {
                            if let GenericParam::Type(type_param) = param {
                                for bound in &mut type_param.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(paren_token) = &trait_bound.paren_token {
                                            let new_trait_bound = TraitBound {
                                                paren_token: Some(token::Paren {
                                                    span: paren_token.span,
                                                }),
                                                modifier: trait_bound.modifier,
                                                lifetimes: trait_bound.lifetimes,
                                                path: syn::Path {
                                                    leading_colon: trait_bound.path.leading_colon,
                                                    segments: {
                                                        let mut segments = Punctuated::new();
                                                        segments.push(syn::PathSegment {
                                                            ident: syn::Ident::new(
                                                                "FnMut",
                                                                Span::call_site(),
                                                            ),
                                                            arguments: trait_bound.path.segments
                                                                .first()
                                                                .unwrap()
                                                                .arguments
                                                                .clone(),
                                                        });
                                                        segments
                                                    },
                                                },
                                            };
                                            *bound = TypeParamBound::Trait(new_trait_bound);
                                        }
                                    }
                                }
                            }
                        }
                        func.sig.generics = generics;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions with higher-ranked trait bounds and changes the trait bound to a different bound, such as changing a bound from `Fn(&'a ())` to `FnMut(&'a ())`. This transformation aims to test the compiler's handling of higher-ranked trait bounds and may lead to ICEs or inference failures in the trait resolution system."
    }
}