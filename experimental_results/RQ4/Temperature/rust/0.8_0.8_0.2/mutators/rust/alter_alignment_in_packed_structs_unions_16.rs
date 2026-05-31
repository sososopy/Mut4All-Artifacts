use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, thread_rng};
use syn::{
    Attribute, File, Fields, FieldsNamed, FieldsUnnamed, Item, ItemStruct, ItemUnion, Meta, MetaList, Path, Type, parse_quote,
    visit_mut::VisitMut,
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
                    if let Some(attr) = find_packed_attr(&mut item_struct.attrs) {
                        let max_alignment = calculate_max_alignment(&item_struct.fields);
                        update_packed_attr(attr, max_alignment);
                    }
                }
                Item::Union(item_union) => {
                    if let Some(attr) = find_packed_attr(&mut item_union.attrs) {
                        let max_alignment = calculate_max_alignment_union(&item_union.fields);
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

fn find_packed_attr(attrs: &mut Vec<Attribute>) -> Option<&mut Attribute> {
    attrs.iter_mut().find(|attr| {
        if let Meta::List(meta_list) = &attr.meta {
            meta_list.path.is_ident("repr")
        } else {
            false
        }
    })
}

fn calculate_max_alignment(fields: &Fields) -> u32 {
    match fields {
        Fields::Named(fields_named) => calculate_max_alignment_for_fields(&fields_named.named),
        Fields::Unnamed(fields_unnamed) => calculate_max_alignment_for_fields(&fields_unnamed.unnamed),
        Fields::Unit => 1,
    }
}

fn calculate_max_alignment_union(fields: &FieldsNamed) -> u32 {
    calculate_max_alignment_for_fields(&fields.named)
}

fn calculate_max_alignment_for_fields(fields: &syn::punctuated::Punctuated<syn::Field, syn::token::Comma>) -> u32 {
    fields.iter().map(|field| {
        match &field.ty {
            Type::Path(type_path) => {
                match type_path.path.segments.last().unwrap().ident.to_string().as_str() {
                    "u8" => 1,
                    "u16" => 2,
                    "u32" => 4,
                    "u64" => 8,
                    _ => 1,
                }
            }
            _ => 1,
        }
    }).max().unwrap_or(1)
}

fn update_packed_attr(attr: &mut Attribute, max_alignment: u32) {
    if let Meta::List(meta_list) = &attr.meta {
        let mut rng = thread_rng();
        let new_alignment = rng.gen_range(1..=max_alignment);
        let tokens = quote! { packed(#new_alignment) };
        let new_meta = syn::parse2::<MetaList>(tokens).expect("Failed to parse tokens into MetaList");
        attr.meta = Meta::List(new_meta);
    }
}