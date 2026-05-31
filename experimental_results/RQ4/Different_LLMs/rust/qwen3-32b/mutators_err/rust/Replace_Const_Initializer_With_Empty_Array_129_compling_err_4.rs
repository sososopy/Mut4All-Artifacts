use syn::parse_quote;
use crate::mutator::Mutator;

pub struct Replace_Const_Initializer_With_Empty_Array_129;

impl Mutator for Replace_Const_Initializer_With_Empty_Array_129 {
    fn name(&self) -> &str {
        "Replace_Const_Initializer_With_Empty_Array_129"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(item_const) = item {
                if let Some(expr) = &mut item_const.expr {
                    *expr = parse_quote! { [] };
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the initializer of const declarations with an empty array literal ([]). This forces the compiler to process destructor handling for zero-sized arrays in const contexts, potentially exposing errors in destructor logic or const evaluation. The transformation is universally applicable to const declarations, regardless of their original value or type, and induces minimal syntactic change to maximize code coverage in the compiler's const processing pipeline."
    }
}