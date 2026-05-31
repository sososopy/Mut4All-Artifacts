use crate::mutator::Mutator;

pub struct Modify_Array_Size_In_Struct_Definition_47;

impl Mutator for Modify_Array_Size_In_Struct_Definition_47 {
    fn name(&self) -> &str {
        "Modify_Array_Size_In_Struct_Definition_47"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { fields, .. }) = item {
                for field in fields {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let Expr::Lit(expr_lit) = &mut *type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                let original_value = lit_int.base10_parse::<usize>().unwrap_or(0);
                                if original_value > 0 {
                                    let new_expr: Expr = parse_quote! {
                                        #original_value / (usize::MAX >> 16)
                                    };
                                    *expr_lit = Expr::Lit(syn::ExprLit {
                                        attrs: Vec::new(),
                                        lit: syn::Lit::Int(syn::LitInt::new(
                                            &new_expr.to_token_stream().to_string(),
                                            lit_int.span(),
                                        )),
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
        "This mutation operator modifies the size of array fields in struct definitions by introducing a division with a large constant. This change aims to explore potential const-eval issues related to excessive memory allocation, encouraging the compiler to handle edge cases where constant evaluation could lead to unintended behavior or crashes."
    }
}