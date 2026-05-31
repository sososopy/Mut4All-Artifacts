use crate::mutator::Mutator;
use syn::{parse_quote, Item, Type, Expr};

pub struct Zero_Sized_Array_In_Union_320;

impl Mutator for Zero_Sized_Array_In_Union_320 {
    fn name(&self) -> &str {
        "Zero_Sized_Array_In_Union_320"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                let mut fields_to_replace = vec![];
                for field in &item_union.fields.named {
                    if let Type::Array(type_array) = &field.ty {
                        if let Expr::Lit(expr_lit) = &type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                if lit_int.base10_parse::<usize>().unwrap_or(1) == 0 {
                                    fields_to_replace.push(field.clone());
                                }
                            }
                        }
                    }
                }
                for field in &fields_to_replace {
                    let new_field: syn::Field = syn::Field {
                        ty: parse_quote!([u8; 1]),
                        ..field.clone()
                    };
                    item_union.fields.named.push(new_field);
                }
                item_union.fields.named = item_union.fields.named.clone().into_iter().filter(|f| !fields_to_replace.contains(f)).collect();
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies unions with zero-sized array fields and replaces these fields with non-zero-sized arrays to test the Rust compiler's handling of union layouts and transmutability."
    }
}