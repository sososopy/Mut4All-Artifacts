use crate::mutator::Mutator;
use syn::{Item, Meta, MetaList, NestedMeta};

pub struct Modify_Struct_Union_Alignments_326;

impl Mutator for Modify_Struct_Union_Alignments_326 {
    fn name(&self) -> &str {
        "Modify_Struct_Union_Alignments_326"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                item_struct.attrs.retain(|attr| {
                    if attr.path().is_ident("repr") {
                        if let Ok(Meta::List(MetaList { nested, .. })) = attr.parse_meta() {
                            return !nested.iter().any(|meta| {
                                if let NestedMeta::Meta(Meta::Path(path)) = meta {
                                    path.is_ident("packed")
                                } else {
                                    false
                                }
                            });
                        }
                    }
                    true
                });
            }
            if let Item::Union(item_union) = item {
                item_union.attrs.retain(|attr| {
                    if attr.path().is_ident("repr") {
                        if let Ok(Meta::List(MetaList { nested, .. })) = attr.parse_meta() {
                            return !nested.iter().any(|meta| {
                                if let NestedMeta::Meta(Meta::Path(path)) = meta {
                                    path.is_ident("packed")
                                } else {
                                    false
                                }
                            });
                        }
                    }
                    true
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies structs and unions with explicit alignment attributes, specifically #[repr(packed)], and removes this attribute. The goal is to expose potential bugs related to data alignment and memory layout assumptions. By removing the packing attribute, the compiler may arrange data differently in memory, revealing inconsistencies and misalignments."
    }
}