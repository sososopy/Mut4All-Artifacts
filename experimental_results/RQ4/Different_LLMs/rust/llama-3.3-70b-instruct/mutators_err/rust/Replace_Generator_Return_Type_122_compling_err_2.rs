impl Mutator for Replace_Generator_Return_Type_122 {
    fn name(&self) -> &str {
        "Replace_Generator_Return_Type_122"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**ty {
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Type::Path(type_path) = &*trait_bound.bounded_ty {
                                    if let Some(segment) = type_path.path.segments.iter().next() {
                                        if segment.ident == "Generator" {
                                            let new_return_type = parse_quote! {
                                                impl Generator<(), Return = i32, Yield = i32>
                                            };
                                            *ty = Box::new(new_return_type);
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
        "The mutation operator replaces the return type of generator functions with a different generator type. It aims to test the compiler's handling of different return types for generators, which may expose bugs related to type checking and code generation."
    }
}