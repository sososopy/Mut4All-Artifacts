struct Replace_Trait_Bounds_411;

impl Replace_Trait_Bounds_411 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_411"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut new_bounds: Punctuated<TypeParamBound, syn::token::Comma> = Punctuated::new();
                let mut rng = rand::thread_rng();

                let action: u8 = rng.gen_range(0..3);

                match action {
                    0 => {
                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: Path {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new("Send", proc_macro2::Span::call_site()),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        }));
                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: Path {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new("Sync", proc_macro2::Span::call_site()),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        }));
                    }
                    1 => {
                        // Remove bounds
                        new_bounds = Punctuated::new();
                    }
                    2 => {
                        // Modify bounds
                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: Path {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new("Send", proc_macro2::Span::call_site()),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        }));
                    }
                    _ => {}
                }
            }
        }
    }
}