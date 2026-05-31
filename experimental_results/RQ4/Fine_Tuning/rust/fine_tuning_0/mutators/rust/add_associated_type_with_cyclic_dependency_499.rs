use syn::{
    punctuated::Punctuated, token, Ident, PathArguments, PathSegment, ReturnType, TraitItem,
    TraitItemType, Type, TypePath,
};
use proc_macro2::Span;
use crate::mutator::Mutator;

pub struct Add_Associated_Type_With_Cyclic_Dependency_499;

impl Mutator for Add_Associated_Type_With_Cyclic_Dependency_499 {
    fn name(&self) -> &str {
        "Add_Associated_Type_With_Cyclic_Dependency_499"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut new_trait_items = Vec::new();
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                let mut bounds = Punctuated::new();
                                for bound in &type_impl_trait.bounds {
                                    bounds.push(bound.clone());
                                }

                                let assoc_type = TraitItemType {
                                    attrs: Vec::new(),
                                    type_token: token::Type {
                                        span: Span::call_site(),
                                    },
                                    ident: Ident::new("AssocType", Span::call_site()),
                                    generics: syn::Generics::default(),
                                    colon_token: Some(token::Colon {
                                        spans: [Span::call_site()],
                                    }),
                                    bounds: bounds.clone(),
                                    default: None,
                                    semi_token: token::Semi {
                                        spans: [Span::call_site()],
                                    },
                                };

                                new_trait_items.push(syn::TraitItem::Type(assoc_type));

                                method.sig.output = ReturnType::Type(
                                    token::RArrow {
                                        spans: [Span::call_site(); 2],
                                    },
                                    Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: {
                                                let mut segments = Punctuated::new();
                                                segments.push(PathSegment {
                                                    ident: Ident::new("Self", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                });
                                                segments.push(PathSegment {
                                                    ident: Ident::new("AssocType", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                });
                                                segments
                                            },
                                        },
                                    })),
                                );
                            }
                        }
                    }
                    new_trait_items.push(trait_item.clone());
                }
                trait_item.items = new_trait_items;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}