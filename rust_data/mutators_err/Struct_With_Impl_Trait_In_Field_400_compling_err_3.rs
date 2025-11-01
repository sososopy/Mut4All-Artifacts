use crate::mutator::Mutator;
use rand::prelude::*;
use rand::thread_rng;
use std::collections::HashSet;
use proc_macro2::{Ident, Span};
use syn::visit::Visit;
use syn::{File, ItemTrait, Item, Type, Field, TypeImplTrait, TypeParamBound, TraitBound, TraitBoundModifier, Path as SynPath, PathSegment, PathArguments};
use syn::punctuated::Punctuated;
use syn::token::{Colon, Impl};

pub struct Struct_With_Impl_Trait_In_Field_400;

impl Mutator for Struct_With_Impl_Trait_In_Field_400 {
    fn name(&self) -> &str {
        "Struct_With_Impl_Trait_In_Field_400"
    }
    fn mutate(&self, file: &mut File) {
        let mut rng = thread_rng();
        let mut trait_candidates = HashSet::new();

        struct TraitCollector<'a> {
            trait_candidates: &'a mut HashSet<Ident>,
        }

        impl<'a> Visit<'_> for TraitCollector<'a> {
            fn visit_item_trait(&mut self, node: &ItemTrait) {
                self.trait_candidates.insert(node.ident.clone());
            }
        }

        let mut collector = TraitCollector {
            trait_candidates: &mut trait_candidates,
        };
        collector.visit_file(file);

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let struct_name = &item_struct.ident;
                let mut selected_field = None;

                for field in &mut item_struct.fields {
                    if let Type::Path(type_path) = &field.ty {
                        if rng.gen_bool(0.5) {
                            selected_field = Some(field);
                            break;
                        }
                    }
                }

                if selected_field.is_none() {
                    if let Some(trait_ident) = trait_candidates.iter().choose(&mut rng) {
                        let new_field_ident = Ident::new(
                            &format!("new_field_{}", rng.gen::<u32>()),
                            Span::call_site(),
                        );
                        if let syn::Fields::Named(fields_named) = &mut item_struct.fields {
                            fields_named.named.push(Field {
                                attrs: Vec::new(),
                                vis: syn::Visibility::Inherited,
                                ident: Some(new_field_ident),
                                colon_token: Some(Colon::default()),
                                ty: Type::ImplTrait(TypeImplTrait {
                                    impl_token: Impl::default(),
                                    bounds: vec![TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: trait_ident.clone(),
                                                arguments: PathArguments::None,
                                            }]),
                                        }
                                    })].into_iter().collect(),
                                }),
                                mutability: None,
                            });
                        }
                    }
                } else if let Some(trait_ident) = trait_candidates.iter().choose(&mut rng) {
                    if let Some(field) = selected_field {
                        field.ty = Type::ImplTrait(TypeImplTrait {
                            impl_token: Impl::default(),
                            bounds: vec![TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: trait_ident.clone(),
                                        arguments: PathArguments::None,
                                    }]),
                                }
                            })].into_iter().collect(),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions, aiming to introduce or modify a field to use `impl Trait`. It first collects all traits defined in the program. Then, for each struct, it either modifies an existing field or introduces a new one with a type of `impl Trait`, where `Trait` is randomly selected from the collected traits. This mutation tests the compiler's handling of trait bounds and associated types without explicit implementations within the struct."
    }
}