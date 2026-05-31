impl Mutator for Replace_Trait_Bounds_411 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_411"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut new_bounds = syn::punctuated::Punctuated::new();
                let mut rng = rand::thread_rng();

                let action: u8 = rng.gen_range(0..3);

                match action {
                    0 => {
                        new_bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                    ident: syn::Ident::new("Send", proc_macro2::Span::call_site()),
                                    arguments: syn::PathArguments::None,
                                }]),
                            },
                        }));
                        new_bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                    ident: syn::Ident::new("Sync", proc_macro2::Span::call_site()),
                                    arguments: syn::PathArguments::None,
                                }]),
                            },
                        }));
                    }
                    1 => {
                        // Remove bounds
                        new_bounds = syn::punctuated::Punctuated::new();
                    }
                    2 => {
                        // Modify bounds
                        new_bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                    ident: syn::Ident::new("Send", proc_macro2::Span::call_site()),
                                    arguments: syn::PathArguments::None,
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