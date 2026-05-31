use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, thread_rng};
use syn::{
    File, Item, ItemStruct, ItemUnion, Meta, MetaList, NestedMeta, parse_quote,
    visit_mut::VisitMut, *,
};
use crate::mutator::Mutator;

pub struct Alter_Alignment_In_Packed_Structs_Unions_16;

impl Mutator for Alter_Alignment_In_Packed_Structs_Unions_16 {
    fn name(&self) -> &str {
        "Alter_Alignment_In_Packed_Structs_Unions_16"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    if let Some(attr) = find_packed_attr(&item_struct.attrs) {
                        let max_alignment = calculate_max_alignment(&item_struct.fields);
                        update_packed_attr(attr, max_alignment);
                    }
                }
                Item::Union(item_union) => {
                    if let Some(attr) = find_packed_attr(&item_union.attrs) {
                        let max_alignment = calculate_max_alignment(&item_union.fields);
                        update_packed_attr(attr, max_alignment);
                    }
                }
                _ => {}
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `#[repr(packed(N))]` attributes in structs and unions. It calculates the maximum natural alignment required by the fields and alters the packed alignment to a random value between 1 and the maximum alignment. This transformation intends to challenge the compiler's handling of layout and alignment by potentially introducing mismatches between ABI and layout alignments."
    }
}

fn find_packed_attr(attrs: &[Attribute]) -> Option<&syn::Attribute> {
    attrs.iter().find(|attr| {
        if let Ok(Meta::List(MetaList { ref path, .. })) = attr.parse_meta() {
            return path.is_ident("repr");
        }
        false
    })
}

fn calculate_max_alignment(fields: &Fields) -> u32 {
    fields.iter().map(|field| {
        match &field.ty {
            Type::Path(type_path) => {
                // Use type_path to determine alignment (e.g., u32 has alignment 4)
                // This is a placeholder; use a proper method to determine alignment
                match type_path.path.segments.last().unwrap().ident.to_string().as_str() {
                    "u8" => 1,
                    "u16" => 2,
                    "u32" => 4,
                    "u64" => 8,
                    _ => 1, // Default fallback
                }
            }
            _ => 1, // Default fallback
        }
    }).max().unwrap_or(1)
}

fn update_packed_attr(attr: &syn::Attribute, max_alignment: u32) {
    if let Ok(Meta::List(MetaList { nested, .. })) = attr.parse_meta() {
        for meta in nested.iter() {
            if let NestedMeta::Meta(Meta::Path(path)) = meta {
                if path.is_ident("packed") {
                    let mut rng = thread_rng();
                    let new_alignment = rng.gen_range(1..=max_alignment);
                    let tokens = quote! { #[repr(packed(#new_alignment))] };
                    *attr.tokens.borrow_mut() = tokens;
                }
            }
        }
    }
}