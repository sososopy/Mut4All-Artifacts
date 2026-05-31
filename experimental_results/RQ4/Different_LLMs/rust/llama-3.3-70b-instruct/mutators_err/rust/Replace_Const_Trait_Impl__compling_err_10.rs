use crate::mutator::Mutator;
use syn::{Item, token};
use proc_macro2::Span;

pub struct ReplaceConstTraitImpl;

impl Mutator for ReplaceConstTraitImpl {
    fn name(&self) -> &str {
        "ReplaceConstTraitImpl"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(unsafety) = &mut impl_item.unsafety {
                    *unsafety = token::Unsafe { span: Span::call_site() };
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const trait implementations with non-const trait implementations. This transformation tests the coherence of const trait implementations and the effects of replacing them with non-const implementations."
    }
}

fn test_mutator() {
    let mut file_ast = syn::parse_quote! {
        // example input
    };
    let mutator = ReplaceConstTraitImpl;
    mutator.mutate(&mut file_ast);
}