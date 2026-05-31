pub struct Introduce_Impl_Trait_Type_Alias_485;

impl Mutator for Introduce_Impl_Trait_Type_Alias_485 {
    fn name(&self) -> &str {
        "Introduce_Impl_Trait_Type_Alias_485"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits = vec![];
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                traits.push(trait_item.ident.clone());
            }
        }

        let selected_trait_path = if !traits.is_empty() {
            let selected_trait = traits.choose(&mut thread_rng()).unwrap();
            let trait_path = SynPath {
                leading_colon: None,
                segments: punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                    ident: selected_trait.clone(),
                    arguments: PathArguments::None,
                }]),
            };
            trait_path
        } else {
            let debug_path = SynPath {
                leading_colon: Some(Default::default()),
                segments: {
                    let mut segments = punctuated::Punctuated::new();
                    segments.push(syn::PathSegment {
                        ident: Ident::new("fmt", Span::call_site()),
                        arguments: PathArguments::None,
                    });
                    segments.push(syn::PathSegment {
                        ident: Ident::new("Debug", Span::call_site()),
                        arguments: PathArguments::None,
                    });
                    segments
                },
            };
            debug_path
        };

        for (i, item) in file.items.iter_mut().enumerate() {
            if let syn::Item::Impl(impl_item) = item {
                if let Type::Path(type_path) = &*impl_item.self_ty {
                    let alias_name = Ident::new("Alias", Span::call_site());
                    let alias_type = Type::ImplTrait(TypeImplTrait {
                        impl_token: token::Impl {
                            span: Span::call_site(),
                        },
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: selected_trait_path.clone(),
                            }));
                            bounds
                        },
                    });

                    let alias_item = Item::Type(syn::ItemType {
                        attrs: vec![],
                        ident: alias_name.clone(),
                        vis: syn::Visibility::Inherited,
                        type_token: Default::default(),
                        generics: Default::default(),
                        eq_token: Default::default(),
                        ty: Box::new(alias_type),
                        semi_token: Default::default(),
                    });

                    file.items.insert(i, alias_item);

                    *impl_item.self_ty = Type::Path(syn::TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments: punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                ident: alias_name,
                                arguments: PathArguments::None,
                            }]),
                        },
                    });

                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}