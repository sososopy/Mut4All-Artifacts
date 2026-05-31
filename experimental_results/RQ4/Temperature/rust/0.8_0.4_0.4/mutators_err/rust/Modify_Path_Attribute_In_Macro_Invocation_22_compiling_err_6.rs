use proc_macro2::{Span, TokenTree};
use quote::*;
use syn::{ExprPath, Ident, Item, parse_quote};

pub struct Modify_Path_Attribute_In_Macro_Invocation_22;

impl Mutator for Modify_Path_Attribute_In_Macro_Invocation_22 {
    fn name(&self) -> &str {
        "Modify_Path_Attribute_In_Macro_Invocation_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                let mut tokens_iter = item_macro.mac.tokens.clone().into_iter();
                if let Some(TokenTree::Ident(ref mut ident)) = tokens_iter.next() {
                    let mut expr_path: ExprPath = parse_quote! { #ident };
                    if let Some(last_segment) = expr_path.path.segments.last_mut() {
                        last_segment.ident = Ident::new("non_existent_path", Span::call_site());
                        item_macro.mac.tokens = quote! { #expr_path };
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}