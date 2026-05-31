impl Mutator for Substitute_Inline_Assembly_Macro_Argument_177 {
    fn name(&self) -> &str {
        "Substitute_Inline_Assembly_Macro_Argument_177"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if mac.mac.path.is_ident("global_asm") {
                    let mut new_tokens = mac.mac.tokens.clone();
                    let mut replaced = false;
                    let mut token_stream = new_tokens.into_iter();
                    while let Some(token) = token_stream.next() {
                        if let TokenTree::Group(group) = token {
                            let mut group_stream = group.stream().into_iter();
                            if let Some(TokenTree::Ident(ident)) = group_stream.next() {
                                if ident.to_string() == "sym" {
                                    let new_expr = parse_quote!({
                                        || {};
                                        0
                                    });
                                    new_tokens = proc_macro2::TokenStream::from(new_expr);
                                    replaced = true;
                                    break;
                                }
                            }
                        }
                    }
                    if replaced {
                        mac.mac.tokens = new_tokens;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes the inline assembly macro argument with a new argument that includes a nested function or closure. This transformation increases the complexity of the code and tests the compiler's ability to handle such constructs, potentially exposing bugs related to inline assembly and nested functions or closures."
    }
}