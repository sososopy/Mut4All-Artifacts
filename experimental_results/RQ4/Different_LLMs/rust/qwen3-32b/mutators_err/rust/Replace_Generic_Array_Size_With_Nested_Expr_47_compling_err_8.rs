use syn::parse_quote;
use crate::mutator::Mutator;
use syn::FnArg;
use syn::Type;
use syn::Expr;

pub struct Replace_Generic_Array_Size_With_Nested_Expr_47;

impl Mutator for Replace_Generic_Array_Size_With_Nested_Expr_47 {
    fn name(&self) -> &str {
        "Replace_Generic_Array_Size_With_Nested_Expr_47"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let ty = &mut pat_type.ty;
                        if let Type::Array(array) = ty {
                            if let Expr::Path(_) = &array.len {
                                let original_len = array.len.clone();
                                let new_len = parse_quote! { (#original_len - 1) + 1 + #original_len };
                                array.len = new_len;
                            }
                        }
                    }
                }
            } else if let syn::Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let Type::Array(array) = field.ty {
                        if let Expr::Path(_) = &array.len {
                            let original_len = array.len.clone();
                            let new_len = parse_quote! { (#original_len - 1) + 1 + #original_len };
                            array.len = new_len;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces direct uses of const generic parameters in array sizes with nested arithmetic expressions combining the same parameter. This introduces overlapping sub-expressions that may confuse const evaluators, testing their ability to handle complex but semantically equivalent expressions during compile-time evaluation."
    }
}