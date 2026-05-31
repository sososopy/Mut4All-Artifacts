use syn::{parse_quote, Item, Attribute, Variant};
use rand::thread_rng;
use rand::Rng;
use crate::mutator::Mutator;

struct ModifyPackedEnumAlignment;

impl Mutator for ModifyPackedEnumAlignment {
    fn name(&self) -> &str {
        "ModifyPackedEnumAlignment"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                for attr in &enum_item.attrs {
                    if attr.path().is_ident("repr") {
                        let mut rng = thread_rng();
                        let mut variants = enum_item.variants.clone();
                        if rng.gen_bool(0.5) {
                            let padding_variant = parse_quote! {
                                _padding: u8,
                            };
                            variants.push(padding_variant);
                        } else {
                            if !variants.is_empty() {
                                let index = rng.gen_range(0..variants.len());
                                variants = variants.into_iter().enumerate().filter_map(|(i, v)| if i == index { None } else { Some(v) }).collect();
                            }
                        }
                        enum_item.variants = variants;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with the #[repr(packed)] attribute and modifies their alignment by adding or removing padding bytes. This transformation aims to test the compiler's handling of enum alignment and size, potentially exposing bugs related to the alignment and size of enums with the #[repr(packed)] attribute."
    }
}