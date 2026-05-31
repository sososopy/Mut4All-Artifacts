use proc_macro2::{Span, Ident};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use std::{collections::HashSet, fs, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Unicode_Identifier_Token_Replacer_13;

impl Mutator for Unicode_Identifier_Token_Replacer_13 {
    fn name(&self) -> &str {
        "Unicode_Identifier_Token_Replacer_13"
    }

    fn mutate(&self, file: &mut syn::File) {
        struct IdentifierVisitor;
        impl VisitMut for IdentifierVisitor {
            fn visit_ident_mut(&mut self, ident: &mut Ident) {
                let mut rng = thread_rng();
                let replacements = [
                    ('a', 'а'), // Cyrillic Small Letter A
                    ('e', 'е'), // Cyrillic Small Letter IE
                    ('i', 'і'), // Cyrillic Small Letter Byelorussian-Ukrainian I
                    // Add more replacements as needed
                ];

                let mut new_ident = ident.to_string();
                for (ascii, unicode) in &replacements {
                    if rng.gen_bool(0.5) {
                        new_ident = new_ident.replace(*ascii, &unicode.to_string());
                    }
                }

                if syn::parse_str::<Ident>(&new_ident).is_ok() && !new_ident.starts_with("r#") {
                    *ident = Ident::new(&new_ident, Spanned::span(ident));
                }
            }
        }

        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    if item_fn.sig.ident != "main" {
                        let mut visitor = IdentifierVisitor;
                        visitor.visit_item_fn_mut(item_fn);
                    }
                }
                Item::Struct(item_struct) => {
                    let mut visitor = IdentifierVisitor;
                    visitor.visit_item_struct_mut(item_struct);
                }
                _ => {}
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets identifiers and tokens in Rust code and replaces some of their ASCII characters with visually similar Unicode characters. This transformation aims to introduce visual confusion while ensuring the code remains syntactically correct, thus testing the compiler's robustness against such changes."
    }
}