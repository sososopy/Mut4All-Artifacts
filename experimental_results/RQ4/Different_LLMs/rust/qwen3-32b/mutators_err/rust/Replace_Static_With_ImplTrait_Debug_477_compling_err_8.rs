pub struct Replace_Static_With_ImplTrait_Debug_477;

impl Mutator for Replace_Static_With_ImplTrait_Debug_477 {
    fn name(&self) -> &str {
        "Replace_Static_With_ImplTrait_Debug_477"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut insertions = Vec::new();
        let mut static_indices = Vec::new();

        for (index, item) in file.items.iter_mut().enumerate() {
            if let syn::Item::Static(static_item) = item {
                if let syn::StaticMutability::Mut(_) = static_item.mutability {
                    let alias_name = Ident::new("Alias_Debug_477", Span::call_site());
                    let alias_item = parse_quote! {
                        pub type #alias_name = impl core::fmt::Debug;
                    };
                    let function_name = Ident::new("dummy_477", Span::call_site());
                    let function_item = parse_quote! {
                        pub fn #function_name() -> #alias_name {
                            Default::default()
                        }
                    };
                    insertions.push( (index, alias_item, function_item, alias_name.clone()) );
                    static_indices.push( (index, alias_name) );
                }
            }
        }

        insertions.sort_by(|a, b| b.0.cmp(&a.0));

        for (index, alias_item, function_item, _) in insertions {
            file.items.insert(index, alias_item);
            file.items.insert(index + 1, function_item);
        }

        for &(index, ref alias_name) in &static_indices {
            if let Some(syn::Item::Static(static_item)) = file.items.get_mut(index) {
                let path = syn::Path {
                    leading_colon: None,
                    segments: Punctuated::from_iter(vec![syn::PathSegment {
                        ident: alias_name.clone(),
                        arguments: syn::PathArguments::None,
                    }]),
                };
                static_item.ty = Box::new(Type::Path(syn::TypePath {
                    qself: None,
                    path,
                }));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}