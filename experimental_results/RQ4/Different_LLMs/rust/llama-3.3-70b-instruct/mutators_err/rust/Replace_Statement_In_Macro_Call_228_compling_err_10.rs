use proc_macro2::{TokenTree, TokenStream, Group, Delimiter};
use syn::parse_quote;

trait Mutator {
    fn mutate(&self, token: &mut TokenTree);
}

struct Replace_Statement_In_Macro_Call_228;

impl Mutator for Replace_Statement_In_Macro_Call_228 {
    fn mutate(&self, token: &mut TokenTree) {
        *token = self.replace_statement(token.clone());
    }
}

impl Replace_Statement_In_Macro_Call_228 {
    fn replace_statement(&self, token: TokenTree) -> TokenTree {
        match token {
            TokenTree::Group(group) => {
                let mut new_group = TokenStream::new();
                for token in group.stream() {
                    match token {
                        TokenTree::Ident(_) | TokenTree::Punct(_) | TokenTree::Literal(_) | TokenTree::Group(_) => {
                            let new_stmt: TokenStream = parse_quote! { let _ = 0; };
                            new_group.extend(vec![TokenTree::Group(Group::new(Delimiter::Brace, new_stmt))]);
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