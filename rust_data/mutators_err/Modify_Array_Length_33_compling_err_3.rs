use crate::mutator::Mutator;

pub struct Modify_Array_Length_33;

impl Mutator for Modify_Array_Length_33 {
    fn name(&self) -> &str {
        "Modify_Array_Length_33"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let Expr::Lit(expr_lit) = &type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                if let Ok(original_len) = lit_int.base10_parse::<usize>() {
                                    let new_len = if thread_rng().gen_bool(0.5) {
                                        original_len + 1
                                    } else {
                                        original_len.saturating_sub(1)
                                    };
                                    type_array.len = Expr::Lit(ExprLit {
                                        attrs: Vec::new(),
                                        lit: syn::Lit::Int(syn::LitInt::new(
                                            &new_len.to_string(),
                                            Span::call_site(),
                                        )),
                                    });
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Impl(item_impl) = item {
                for item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = item {
                        for stmt in &mut method.block.stmts {
                            if let Stmt::Expr(Expr::Struct(expr_struct)) = stmt {
                                for field in &mut expr_struct.fields {
                                    if let Expr::Array(expr_array) = &mut field.expr {
                                        if let Expr::Repeat(expr_repeat) = &mut expr_array.elems[0] {
                                            if let Expr::Lit(expr_lit) = &*expr_repeat.len {
                                                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                    if let Ok(original_len) = lit_int.base10_parse::<usize>() {
                                                        let new_len = if thread_rng().gen_bool(0.5) {
                                                            original_len + 1
                                                        } else {
                                                            original_len.saturating_sub(1)
                                                        };
                                                        expr_repeat.len = Box::new(Expr::Lit(ExprLit {
                                                            attrs: Vec::new(),
                                                            lit: syn::Lit::Int(syn::LitInt::new(
                                                                &new_len.to_string(),
                                                                Span::call_site(),
                                                            )),
                                                        }));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the length of array fields in struct initializations. It targets struct definitions where array lengths are defined using constant expressions. The mutation involves changing the array length by either increasing or decreasing it by 1. This can reveal bugs in the compiler's handling of array sizes and layout determination during constant evaluation."
    }
}