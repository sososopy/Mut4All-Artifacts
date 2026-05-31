use syn::{parse_quote, Attribute, Item, Meta, MetaList, token};
use proc_macro2::Span;
use quote::quote;
use crate::mutator::Mutator;

struct Change_Enum_Alignment_461;

impl Mutator for Change_Enum_Alignment_461 {
    fn name(&self) -> &str {
        "Change_Enum_Alignment_461"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let alignment = 2; 
                let attribute = Attribute {
                    pound_token: token::Pound {
                        spans: [Span::call_site()],
                    },
                    style: syn::AttrStyle::Inner(Default::default()),
                    bracket_token: Default::default(),
                    meta: Meta::List(MetaList {
                        path: parse_quote!(repr),
                        tokens: Default::default(),
                        delimiter: syn::token::Comma, 
                    }),
                };
                if let Item::Enum(item_enum) = item {
                    let mut new_attribute = attribute.clone();
                    new_attribute.meta = Meta::List(MetaList {
                        path: parse_quote!(repr),
                        tokens: quote!(C, packed(#alignment)).into(),
                        delimiter: syn::token::Comma, 
                    });
                    item_enum.attrs.push(new_attribute);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the alignment of enums by adding or modifying the `#[repr(C, packed(n))]` attribute. This transformation aims to trigger bugs related to alignment mismatches and test the compiler's handling of enum layout and ABI compatibility."
    }
}