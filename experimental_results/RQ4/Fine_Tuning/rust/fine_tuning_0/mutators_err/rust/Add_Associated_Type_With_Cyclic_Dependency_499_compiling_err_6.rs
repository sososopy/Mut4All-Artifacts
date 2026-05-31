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
                                        spans: [Span::call_site(), Span::call_site()],
                                    },
                                };

                                if let syn::TraitItem::Type(_) = trait_item {
                                    *trait_item = syn::TraitItem::Type(assoc_type);
                                }

                                method.sig.output = ReturnType::Type(
                                    token::RArrow {
                                        spans: [Span::call_site()],
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}