use syn::parse_quote;
use syn::{Item, Meta, Attribute};
use syn::punctuated::Punctuated;
use crate::mutator::Mutator;
use rand::Rng;
use rand::prelude::SliceRandom;

pub struct Modify_Enum_Alignment_428;

impl Mutator for Modify_Enum_Alignment_428 {
    fn name(&self) -> &str {
        "Modify_Enum_Alignment_428"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut new_attrs = Vec::new();
                for attr in &item_enum.attrs {
                    if attr.path().is_ident("repr") {
                        let mut has_packed = false;
                        let mut new_meta_list = Punctuated::new();
                        let _ = attr.parse_nested_meta(|nested| {
                            if let Meta::Path(path) = &nested {
                                if path.is_ident("C") {
                                    new_meta_list.push(parse_quote!(#nested));
                                }
                            } else if let Meta::List(meta_list) = &nested {
                                if meta_list.path.is_ident("packed") {
                                    has_packed = true;
                                } else {
                                    new_meta_list.push(parse_quote!(#nested));
                                }
                            }
                            Ok(())
                        });
                        if has_packed {
                            if rand::thread_rng().gen_bool(0.5) {
                                new_attrs.push(parse_quote!(#[repr(C)]));
                            } else {
                                let align_value = [2, 4, 8].choose(&mut rand::thread_rng()).unwrap();
                                new_attrs.push(parse_quote!(#[repr(C, align(#align_value))]));
                            }
                        } else {
                            new_attrs.push(attr.clone());
                        }
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }
                item_enum.attrs = new_attrs;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies enums with a `#[repr(C, packed(N))]` attribute and modifies the packing alignment or removes it. The mutation either changes the packing to a different alignment value or removes the packing attribute, replacing it with `#[repr(C)]` or `#[repr(C, align(M))]`. This approach tests different alignment strategies to identify potential ABI and layout mismatches."
    }
}