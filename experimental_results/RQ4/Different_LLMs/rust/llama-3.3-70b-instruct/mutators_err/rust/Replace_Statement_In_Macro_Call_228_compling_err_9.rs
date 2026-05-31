use proc_macro2::{TokenTree, TokenStream, Group, Delimiter};
use syn::parse_quote;

struct Replace_Statement_In_Macro_Call_228;

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
                TokenTree::Group(Group::new(Delimiter::Brace, new_group))
            }
            _ => token,
        }
    }
}