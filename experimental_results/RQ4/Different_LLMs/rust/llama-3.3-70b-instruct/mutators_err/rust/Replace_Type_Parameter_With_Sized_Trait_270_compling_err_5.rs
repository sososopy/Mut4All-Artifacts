use syn::{GenericParam, TypeParamBound, TraitBound, TraitBoundModifier, Path, PathSegment, Ident, Span, PathArguments, punctuated::Punctuated};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Replace_Type_Parameter_With_Sized_Trait_270;

impl Mutator for Replace_Type_Parameter_With_Sized_Trait_270 {
    fn name(&self) -> &str {
        "Replace_Type_Parameter_With_Sized_Trait_270"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                match &mut struct_item.generics {
                    syn::Generics::None => {
                        struct_item.generics = syn::Generics::default();
                    }
                    _ => {}
                }
                let generics = match &mut struct_item.generics {
                    syn::Generics::None => {
                        struct_item.generics = syn::Generics::default();
                        &mut struct_item.generics
                    }
                    g => g,
                };
                for param in &mut generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(paren_token) = &trait_bound.paren_token {
                                    if trait_bound.path.is_ident("Sized") {
                                        let my_trait_bound = TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("MyTrait", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        };
                                        let sized_trait_bound = TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("Sized", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        };
                                        let mut new_bounds = Punctuated::new();
                                        new_bounds.push(TypeParamBound::Trait(my_trait_bound));
                                        new_bounds.push(TypeParamBound::Trait(sized_trait_bound));
                                        type_param.bounds = new_bounds;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Trait(trait_item) = item {
                match &mut trait_item.generics {
                    syn::Generics::None => {
                        trait_item.generics = syn::Generics::default();
                    }
                    _ => {}
                }
                let generics = match &mut trait_item.generics {
                    syn::Generics::None => {
                        trait_item.generics = syn::Generics::default();
                        &mut trait_item.generics
                    }
                    g => g,
                };
                for param in &mut generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(paren_token) = &trait_bound.paren_token {
                                    if trait_bound.path.is_ident("Sized") {
                                        let my_trait_bound = TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("MyTrait", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        };
                                        let sized_trait_bound = TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("Sized", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        };
                                        let mut new_bounds = Punctuated::new();
                                        new_bounds.push(TypeParamBound::Trait(my_trait_bound));
                                        new_bounds.push(TypeParamBound::Trait(sized_trait_bound));
                                        type_param.bounds = new_bounds;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets type parameters in struct and trait definitions and replaces them with a trait that extends the Sized trait. The goal is to check if the compiler can correctly handle the replacement and still maintain the correctness of the code."
    }
}