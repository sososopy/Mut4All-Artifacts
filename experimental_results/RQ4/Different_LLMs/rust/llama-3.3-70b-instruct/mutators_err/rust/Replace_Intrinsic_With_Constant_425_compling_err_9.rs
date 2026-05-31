impl Mutator for Replace_Intrinsic_With_Constant_425 {
    fn name(&self) -> &str {
        "Replace_Intrinsic_With_Constant_425"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let syn::Expr::Call(ExprCall {
                    expr: syn::Expr::Path(ExprPath {
                        path: syn::Path {
                            segments: punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                ident: syn::Ident::new("transmute", proc_macro2::Span::call_site()),
                                arguments: syn::PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: proc_macro2::Token![<](proc_macro2::Span::call_site()),
                                    args: punctuated::Punctuated::from_iter(vec![syn::GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                        qself: None,
                                        path: syn::Path::from(syn::Ident::new("u8", proc_macro2::Span::call_site())),
                                    }))]),
                                    gt_token: proc_macro2::Token![>](proc_macro2::Span::call_site()),
                                }),
                            }]),
                            ..
                        }),
                        ..
                    }),
                    ..
                }) = &const_item.expr {
                    const_item.expr = syn::parse_quote! { 0u8 };
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the use of intrinsic functions within constant expressions with constant values. This transformation tests the compiler's handling of constant expressions and intrinsic functions, and may expose potential bugs in the compiler's constant evaluation and code generation."
    }
}