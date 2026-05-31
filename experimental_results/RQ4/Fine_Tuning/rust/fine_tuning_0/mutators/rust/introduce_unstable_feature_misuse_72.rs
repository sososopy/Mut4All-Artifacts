use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, thread_rng};
use syn::{
    Expr, ExprLit, File, Item, LitInt, parse_quote, token, ItemEnum, ItemFn,
};

use crate::mutator::Mutator;

pub struct Introduce_Unstable_Feature_Misuse_72;

impl Mutator for Introduce_Unstable_Feature_Misuse_72 {
    fn name(&self) -> &str {
        "Introduce_Unstable_Feature_Misuse_72"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut has_arbitrary_enum_discriminant = false;
        let mut rng = thread_rng();
        let mut new_items = Vec::new();

        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_custom_discriminants = false;
                for variant in &item_enum.variants {
                    if variant.discriminant.is_some() {
                        has_custom_discriminants = true;
                        break;
                    }
                }

                if !has_custom_discriminants {
                    for variant in &mut item_enum.variants {
                        let discriminant_value: i32 = rng.gen_range(-100..100);
                        variant.discriminant = Some((
                            token::Eq { spans: [Span::call_site()] },
                            Expr::Lit(ExprLit {
                                attrs: Vec::new(),
                                lit: syn::Lit::Int(LitInt::new(&discriminant_value.to_string(), Span::call_site())),
                            }),
                        ));
                    }
                }

                if !has_arbitrary_enum_discriminant {
                    new_items.push(Item::Verbatim(quote! {
                        #![feature(arbitrary_enum_discriminant)]
                    }));
                    has_arbitrary_enum_discriminant = true;
                }
            }
        }

        if !has_arbitrary_enum_discriminant {
            new_items.push(Item::Verbatim(quote! {
                #![feature(arbitrary_enum_discriminant)]
            }));
        }

        let faulty_enum: ItemEnum = parse_quote! {
            enum Faulty {
                C = 0,
                D(!),
            }
        };

        new_items.push(Item::Enum(faulty_enum));

        let usage_fn: ItemFn = parse_quote! {
            fn usage() {
                let _ = Faulty::D; // Improper use of `!` type causes instability
            }
        };

        new_items.push(Item::Fn(usage_fn));

        file.items.splice(0..0, new_items);
    }

    fn chain_of_thought(&self) -> &str {
        ""
    }
}