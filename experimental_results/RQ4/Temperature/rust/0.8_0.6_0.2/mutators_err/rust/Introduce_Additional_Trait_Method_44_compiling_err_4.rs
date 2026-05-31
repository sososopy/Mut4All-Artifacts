use proc_macro2::{Span, *};
use quote::*;
use syn::{
    File, Ident, Item, ItemTrait, TraitItem, Type, ReturnType, FnArg, PatType, parse_quote,
    punctuated::Punctuated, token, TraitItemMethod,
};
use crate::mutator::Mutator;

pub struct Introduce_Additional_Trait_Method_44;

impl Mutator for Introduce_Additional_Trait_Method_44 {
    fn name(&self) -> &str {
        "Introduce_Additional_Trait_Method_44"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.items.iter().any(|i| matches!(i, TraitItem::Method(_))) {
                    let new_method = TraitItem::Method(TraitItemMethod {
                        attrs: Vec::new(),
                        sig: parse_quote! {
                            fn new_method(x: i32, y: i32) -> String
                        },
                        default: None,
                        semi_token: Some(token::Semi::default()),
                    });
                    trait_item.items.push(new_method);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}