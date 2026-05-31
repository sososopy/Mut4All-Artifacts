impl Mutator for Swap_Const_Generics_And_Type_Generics_108 {
    fn name(&self) -> &str {
        "Swap_Const_Generics_And_Type_Generics_108"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let syn::Generics::None = struct_item.generics {
                    continue;
                }
                let generics = struct_item.generics_mut().unwrap();
                let mut const_generics = Vec::new();
                let mut type_generics = Vec::new();
                for param in &mut generics.params {
                    match param {
                        GenericParam::Const(_) => const_generics.push(param.clone()),
                        GenericParam::Type(_) => type_generics.push(param.clone()),
                        _ => {}
                    }
                }
                generics.params.clear();
                generics.params.extend(type_generics);
                generics.params.extend(const_generics);
            }
            if let syn::Item::Enum(enum_item) = item {
                if let syn::Generics::None = enum_item.generics {
                    continue;
                }
                let generics = enum_item.generics_mut().unwrap();
                let mut const_generics = Vec::new();
                let mut type_generics = Vec::new();
                for param in &mut generics.params {
                    match param {
                        GenericParam::Const(_) => const_generics.push(param.clone()),
                        GenericParam::Type(_) => type_generics.push(param.clone()),
                        _ => {}
                    }
                }
                generics.params.clear();
                generics.params.extend(type_generics);
                generics.params.extend(const_generics);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of const generic parameters and type generic parameters in a generic struct or enum definition. This transformation tests the compiler's ability to handle the scoping and evaluation of const expressions in the context of generic definitions."
    }
}