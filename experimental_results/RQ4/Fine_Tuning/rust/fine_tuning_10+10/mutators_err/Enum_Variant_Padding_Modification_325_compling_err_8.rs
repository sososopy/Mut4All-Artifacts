use syn::{parse_quote, Item, ItemStruct};
use rand::thread_rng;
use proc_macro2::{Ident, Span};
use crate::mutator::Mutator;

pub struct EnumVariantPaddingModification325;

impl Mutator for EnumVariantPaddingModification325 {
    fn name(&self) -> &str {
        "EnumVariantPaddingModification325"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_repr = false;
                for attr in &item_enum.attrs {
                    if attr.path.is_ident("repr") {
                        has_repr = true;
                        break;
                    }
                }
                if !has_repr {
                    item_enum.attrs.push(parse_quote!(#[repr(C)]));
                }

                let mut rng = thread_rng();
                let new_variant = Ident::new("C", Span::call_site());
                let new_struct: ItemStruct = parse_quote! {
                    #[repr(C)]
                    struct NewStruct {
                        a: u8,
                        b: u16,
                    }
                };
                item_enum.variants.push(parse_quote! {
                    #new_variant(NewStruct)
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum definitions with multiple variants, aiming to expose potential mismanagement issues in layout and padding. It modifies the representation attribute of the enum to enforce specific alignments and introduces a new variant with a struct that has a different alignment requirement. This change can highlight bugs in the compiler's handling of enum variant sizes and alignments."
    }
}