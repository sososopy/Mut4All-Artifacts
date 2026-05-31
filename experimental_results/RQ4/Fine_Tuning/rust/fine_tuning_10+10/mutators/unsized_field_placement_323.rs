use syn::{parse_quote, Item, Fields, Type, PathSegment, PathArguments};
use proc_macro2::{Ident, Span};
use rand::thread_rng;
use rand::prelude::SliceRandom;
use crate::mutator::Mutator;

pub struct Unsized_Field_Placement_323;

impl Mutator for Unsized_Field_Placement_323 {
    fn name(&self) -> &str {
        "Unsized_Field_Placement_323"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Fields::Named(fields_named) = &mut item_struct.fields {
                    if fields_named.named.len() > 1 {
                        let mut rng = thread_rng();
                        let mut sized_field_indices: Vec<usize> = fields_named.named.iter().enumerate()
                            .filter(|(_, field)| matches!(field.ty, Type::Path(_)))
                            .map(|(i, _)| i)
                            .collect();
                        
                        if sized_field_indices.len() > 1 {
                            sized_field_indices.shuffle(&mut rng);
                            let selected_index = sized_field_indices[0];
                            
                            if selected_index > 0 {
                                if let Type::Path(type_path) = &mut fields_named.named[selected_index].ty {
                                    type_path.path.segments.last_mut().unwrap().ident = Ident::new("u8", Span::call_site());
                                    type_path.path.segments.push(PathSegment {
                                        ident: Ident::new("Slice", Span::call_site()),
                                        arguments: PathArguments::AngleBracketed(parse_quote!(<u8>)),
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies struct definitions with multiple fields and selects a field with a Sized type that is not the first field. It changes this field's type to an unsized type, such as a slice. This mutation can trigger layout issues due to Rust's constraints on unsized fields."
    }
}