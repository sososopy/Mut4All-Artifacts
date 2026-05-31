use syn::{parse_quote, Item, Expr};
use proc_macro2::TokenTree;
use quote::quote;
use crate::mutator::Mutator;

struct ReplaceMacroInvocationWithExpansion_151;

impl Mutator for ReplaceMacroInvocationWithExpansion_151 {
    fn name(&self) -> &str {
        "ReplaceMacroInvocationWithExpansion_151"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                let mut expanded_code = String::new();
                let mut tokens = mac.mac.tokens.clone();
                for token in tokens {
                    if let TokenTree::Ident(ident) = token {
                        expanded_code.push_str(&ident.to_string());
                    } else if let TokenTree::Punct(punct) = token {
                        expanded_code.push_str(&punct.to_string());
                    } else if let TokenTree::Literal(literal) = token {
                        expanded_code.push_str(&literal.to_string());
                    } else if let TokenTree::Group(group) = token {
                        expanded_code.push_str(&group.to_string());
                    }
                }
                let expanded_code = parse_quote!(#expanded_code);
                *item = Item::Expr(Expr::Verbatim(quote!(#expanded_code)));
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations in the code and replaces them with their expanded forms. This transformation aims to test the compiler's ability to handle the expanded code and identify potential issues that may arise during the expansion process."
    }
}