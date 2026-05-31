use proc_macro2::{TokenTree, Group, TokenStream};
use quote::quote;
use syn::{Item, Expr, parse_quote};
use crate::mutator::Mutator;

pub struct Misplaced_Crate_In_Macro_597;

impl Mutator for Misplaced_Crate_In_Macro_597 {
    fn name(&self) -> &str {
        "Misplaced_Crate_In_Macro_597"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some(TokenTree::Group(ref mut group)) = mac.mac.tokens.clone().into_iter().next() {
                    let misplaced_crate: Expr = parse_quote! { $crate };
                    let mut new_stream = TokenStream::new();
                    new_stream.extend(group.stream());
                    new_stream.extend(quote! { #misplaced_crate; });
                    *group = Group::new(group.delimiter(), new_stream);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro definitions and introduces `$crate` in unexpected positions within the macro body. This aims to test the compiler's handling of `$crate` when used outside of its typical context, potentially leading to path resolution errors or internal compiler errors."
    }
}