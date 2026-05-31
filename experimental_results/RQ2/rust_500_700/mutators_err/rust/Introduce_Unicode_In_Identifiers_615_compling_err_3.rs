use proc_macro2::{Span, Ident};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use std::{fs, path::Path, process::Command};
use syn::{
    File, Item, ItemFn, ItemStruct, parse_quote,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Introduce_Unicode_In_Identifiers_615;

impl Mutator for Introduce_Unicode_In_Identifiers_615 {
    fn name(&self) -> &str {
        "Introduce_Unicode_In_Identifiers_615"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let unicode_chars = vec!['ä', 'å', 'ç', 'é', 'ñ', 'ü'];
        
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    let new_ident = introduce_unicode(&item_fn.sig.ident, &unicode_chars, &mut rng);
                    item_fn.sig.ident = new_ident;
                }
                Item::Struct(item_struct) => {
                    let new_ident = introduce_unicode(&item_struct.ident, &unicode_chars, &mut rng);
                    item_struct.ident = new_ident;
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces Unicode characters into identifiers, such as function and struct names, to test the Rust compiler's handling of Unicode in parsing and symbol resolution. By adding diacritics and other non-ASCII characters, it aims to uncover issues related to Unicode support in the compiler."
    }
}

fn introduce_unicode(ident: &Ident, unicode_chars: &[char], rng: &mut impl Rng) -> Ident {
    let mut new_name = ident.to_string();
    let insert_pos = rng.gen_range(0..=new_name.len());
    let unicode_char = unicode_chars.choose(rng).unwrap();
    new_name.insert(insert_pos, *unicode_char);
    Ident::new(&format!("r#{}", new_name), Span::call_site())
}