impl Mutator for Replace_Trait_Bounds_With_Supertraits_276 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_With_Supertraits_276"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for bound in &mut trait_item.supertraits {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(path) = trait_bound.path.segments.last() {
                            let supertraits = get_supertraits(trait_bound.path.clone());
                            if let Some(supertrait) = supertraits.choose(&mut thread_rng()) {
                                *bound = TypeParamBound::Trait(TraitBound {
                                    paren_token: trait_bound.paren_token,
                                    lifetimes: trait_bound.lifetimes.clone(),
                                    path: supertrait.clone(),
                                    modifier: trait_bound.modifier,
                                });
                            }
                        }
                    }
                }
            } else if let Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = &mut impl_item.trait_ {
                    for segment in &mut path.segments {
                        match &mut segment.arguments {
                            PathArguments::AngleBracketed(args) => {
                                for arg in &mut args.args {
                                    if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                        let supertraits = get_supertraits(type_path.path.clone());
                                        if let Some(supertrait) = supertraits.choose(&mut thread_rng()) {
                                            *arg = GenericArgument::Type(Type::Path(TypePath {
                                                qself: None,
                                                path: supertrait.clone(),
                                            }));
                                        }
                                    }
                                }
                            }
                            _ => {}
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds with their supertraits. This transformation tests the robustness of the trait system and can expose bugs related to trait bounds and supertraits."
    }
}

fn get_supertraits(path: syn::Path) -> Vec<syn::Path> {
    let mut supertraits = Vec::new();
    match path.segments.last().unwrap().ident.to_string().as_str() {
        "Iterator" => {
            supertraits.push(parse_quote!(std::iter::ExactSizeIterator));
            supertraits.push(parse_quote!(std::iter::DoubleEndedIterator));
        }
        "ExactSizeIterator" => {
            supertraits.push(parse_quote!(std::iter::Iterator));
        }
        "DoubleEndedIterator" => {
            supertraits.push(parse_quote!(std::iter::Iterator));
        }
        _ => {}
    }
    supertraits
}