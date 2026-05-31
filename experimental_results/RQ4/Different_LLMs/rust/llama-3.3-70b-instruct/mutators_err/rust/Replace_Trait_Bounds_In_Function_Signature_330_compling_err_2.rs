impl Mutator for Replace_Trait_Bounds_In_Function_Signature_330 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_In_Function_Signature_330"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut trait_bounds = vec!["Send", "Sync", "Debug", "Clone", "Default"];
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            let bounds = &type_impl_trait.bounds;
                            if bounds.len() > 0 {
                                let bound = trait_bounds.choose(&mut thread_rng()).unwrap();
                                let new_bound = TypeParamBound::Trait(TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new(bound, Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                });
                                let mut new_bounds: Punctuated<TypeParamBound, Comma> = Punctuated::new();
                                new_bounds.push(new_bound);
                                pat_type.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: type_impl_trait.impl_token.clone(),
                                    bounds: new_bounds,
                                }));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds in function signatures with a different trait bound. This transformation tests the compiler's ability to handle different trait bounds and may lead to bugs related to trait bounds and the `dyn_star` feature."
    }
}