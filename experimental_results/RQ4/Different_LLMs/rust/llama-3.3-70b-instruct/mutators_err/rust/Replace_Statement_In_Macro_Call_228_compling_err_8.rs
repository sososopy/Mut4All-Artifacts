impl Replace_Statement_In_Macro_Call_228 {
    fn replace_statement(&self, token: TokenTree) -> TokenTree {
        match token {
            TokenTree::Group(group) => {
                let mut new_group = TokenStream::new();
                for token in group.stream() {
                    match token {
                        TokenTree::Ident(_) | TokenTree::Punct(_) | TokenTree::Literal(_) | TokenTree::Group(_) => {
                            let new_stmt = parse_quote! { let _ = 0; };
                            new_group.extend(vec![new_stmt]);
                        }
                        _ => new_group.extend(vec![token.clone()]),
                    }
                }
                TokenTree::Group(proc_macro2::Group::new(proc_macro2::Delimiter::Brace, new_group))
            }
            _ => token,
        }
    }
}