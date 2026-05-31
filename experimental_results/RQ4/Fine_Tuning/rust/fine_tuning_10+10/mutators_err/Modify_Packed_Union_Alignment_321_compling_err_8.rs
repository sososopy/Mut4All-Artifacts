use crate::mutator::Mutator;
use quote::quote;
use syn::{parse_quote, Item, Lit, LitInt};

pub struct Modify_Packed_Union_Alignment_321;

impl Mutator for Modify_Packed_Union_Alignment_321 {
    fn name(&self) -> &str {
        "Modify_Packed_Union_Alignment_321"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                if let Some(attr) = item_union.attrs.iter_mut().find(|attr| attr.path.is_ident("repr")) {
                    let mut new_meta = None;
                    attr.parse_nested_meta(|meta| {
                        if meta.path.is_ident("packed") {
                            if let Ok(Lit::Int(lit_int)) = meta.value().and_then(|v| v.parse::<Lit>()) {
                                if let Ok(n) = lit_int.base10_parse::<u64>() {
                                    let new_n = n + 1;
                                    let lit_int_new = LitInt::new(&new_n.to_string(), lit_int.span());
                                    let tokens = quote!((packed(#lit_int_new)));
                                    new_meta = Some(parse_quote!(#tokens));
                                }
                            }
                        }
                        Ok(())
                    }).ok();
                    if let Some(meta) = new_meta {
                        attr.meta = meta;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets union type declarations with the #[repr(packed(n))] attribute. It identifies the current packing alignment value 'n' and modifies it to a larger value, potentially causing misalignment. The new value is chosen to be larger than the natural alignment of any field within the union, ensuring it remains within valid range."
    }
}