use rand::thread_rng;
use rand::seq::SliceRandom;
use syn::{File, Item, ItemEnum};

struct Change_Enum_Variant_Order_72;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Change_Enum_Variant_Order_72 {
    fn name(&self) -> &str {
        "Change_Enum_Variant_Order_72"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut variants: Vec<_> = enum_item.variants.iter().cloned().collect();
                variants.shuffle(&mut thread_rng());
                enum_item.variants = variants.into_iter().collect();
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the order of enum variants. This transformation tests the compiler's handling of enum layouts and may expose bugs related to transmutability and discriminant values."
    }
}

fn main() {
    let mut file = File {
        shebang: None,
        attrs: vec![],
        items: vec![ItemEnum(syn::ItemEnum {
            attrs: vec![],
            vis: syn::Visibility::Inherited,
            enum_token: syn::token::Enum,
            ident: syn::Ident::new("TestEnum", syn::Span::call_site()),
            generics: syn::Generics::default(),
            brace_token: syn::token::Brace,
            variants: vec![
                syn::Variant {
                    attrs: vec![],
                    ident: syn::Ident::new("Variant1", syn::Span::call_site()),
                    fields: syn::Fields::Unit,
                    discriminant: None,
                },
                syn::Variant {
                    attrs: vec![],
                    ident: syn::Ident::new("Variant2", syn::Span::call_site()),
                    fields: syn::Fields::Unit,
                    discriminant: None,
                },
            ],
        })],
    };

    let mutator = Change_Enum_Variant_Order_72;
    mutator.mutate(&mut file);
}