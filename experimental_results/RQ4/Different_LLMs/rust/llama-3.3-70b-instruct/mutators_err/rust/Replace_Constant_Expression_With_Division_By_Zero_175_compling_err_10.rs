use proc_macro2::{Span, TokenStream, TokenTree};
use syn::{parse_quote, File, Item, ItemMacro};
use crate::mutator::Mutator;

struct Replace_Constant_Expression_With_Division_By_Zero_175;

impl Mutator for Replace_Constant_Expression_With_Division_By_Zero_175 {
    fn name(&self) -> &str {
        "Replace_Constant_Expression_With_Division_By_Zero_175"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                let mut new_stream = TokenStream::new();
                for token in mac.mac.tokens.clone() {
                    if let TokenTree::Group(group) = token {
                        let mut new_group = group.clone();
                        new_group.set_span(Span::call_site());
                        let new_stream_group = replace_constant_expressions(new_group.stream());
                        new_group.stream = new_stream_group;
                        new_stream.extend(vec![TokenTree::Group(new_group)]);
                    } else {
                        new_stream.extend(vec![token.clone()]);
                    }
                }
                mac.mac.tokens = new_stream;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions within the global_asm macro and replaces them with a division by zero expression, which can trigger the internal compiler error."
    }
}

fn replace_constant_expressions(stream: TokenStream) -> TokenStream {
    let mut new_stream = TokenStream::new();
    for token in stream {
        if let TokenTree::Ident(_) = token {
            let new_expr: TokenStream = parse_quote!(1 / 0);
            new_stream.extend(new_expr);
        } else if let TokenTree::Literal(_) = token {
            let new_expr: TokenStream = parse_quote!(1 / 0);
            new_stream.extend(new_expr);
        } else {
            new_stream.extend(vec![token.clone()]);
        }
    }
    new_stream
}