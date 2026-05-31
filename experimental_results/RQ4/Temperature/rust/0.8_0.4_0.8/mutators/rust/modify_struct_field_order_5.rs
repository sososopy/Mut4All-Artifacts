use syn::token::Brace;
use crate::mutator::Mutator;
use syn::{Item, Fields};
use rand::thread_rng;
use proc_macro2::Span;
use syn::punctuated::Punctuated;
use rand::seq::SliceRandom;

pub struct Modify_Struct_Field_Order_5;

impl Mutator for Modify_Struct_Field_Order_5 {
    fn name(&self) -> &str {
        "Modify_Struct_Field_Order_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Fields::Named(ref mut fields) = item_struct.fields {
                    if fields.named.len() > 1 {
                        let mut rng = thread_rng();
                        let mut field_vec: Vec<_> = fields.named.iter().cloned().collect();
                        field_vec.shuffle(&mut rng);
                        fields.named = Punctuated::from_iter(field_vec);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator reorders the fields within struct definitions. This transformation aims to reveal bugs related to field order assumptions in struct initialization, field access, and type handling. By altering the field order, it challenges the compiler's ability to correctly manage struct layouts and access patterns."
    }
}