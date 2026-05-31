pub struct Introduce_Undeclared_Lifetime_In_Const_Generic_Return_105;

impl Mutator for Introduce_Undeclared_Lifetime_In_Const_Generic_Return_105 {
    fn name(&self) -> &str {
        "Introduce_Undeclared_Lifetime_In_Const_Generic_Return_105"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let Type::Path(type_path) = &mut **return_type {
                                let mut has_angle_bracketed = false;
                                for segment in &type_path.path.segments {
                                    if let PathArguments::AngleBracketed(_) = &segment.arguments {
                                        has_angle_bracketed = true;
                                        break;
                                    }
                                }
                                if has_angle_bracketed {
                                    for segment in &mut type_path.path.segments {
                                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                            let new_lifetime = Lifetime::new("'a", Span::call_site());
                                            args.args.insert(0, GenericArgument::Lifetime(new_lifetime));
                                        }
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
        ""
    }
}