impl Mutator for Replace_Const_Generic_With_Non_Const_Equivalent_102 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Non_Const_Equivalent_102"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Const(const_item) = item {
                        let const_name = &const_item.ident;
                        let const_ty = &const_item.ty;
                        let new_method = parse_quote! {
                            fn #const_name(&self) -> #const_ty;
                        };
                        trait_item.items.push(syn::TraitItem::Method(syn::MethodSig {
                            constness: None,
                            unsafety: None,
                            abi: None,
                            ident: const_name.clone(),
                            generativity: None,
                            args: parse_quote! { ( &self ) },
                            varargs: None,
                            output: parse_quote! { -> #const_ty },
                        }));
                        trait_item.items.retain(|item| {
                            if let syn::TraitItem::Const(const_item) = item {
                                const_item.ident != *const_name
                            } else {
                                true
                            }
                        });
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic expressions with non-const equivalents. It targets const generic expressions in Rust code and replaces them with non-const equivalents, such as runtime-computed values or non-generic constants. This transformation aims to identify potential bugs in the Rust compiler related to const generics."
    }
}