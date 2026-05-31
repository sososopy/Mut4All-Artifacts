use syn::{parse_quote, GenericParam, Expr, Lit};
use crate::mutator::Mutator;

struct Replace_Constant_With_Constant_Expression_84;

impl Mutator for Replace_Constant_With_Constant_Expression_84 {
    fn name(&self) -> &str {
        "Replace_Constant_With_Constant_Expression_84"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let new_expr = match const_param.default {
                            Some(ref default_expr) => {
                                let new_expr = match default_expr {
                                    Expr::Lit(lit_expr) => {
                                        let lit = &lit_expr.lit;
                                        match lit {
                                            Lit::Int(int_lit) => {
                                                let new_int = int_lit.base10_parse::<i32>().unwrap() * 2;
                                                parse_quote! { #new_int }
                                            }
                                            Lit::Float(float_lit) => {
                                                let new_float = float_lit.base10_parse::<f64>().unwrap() * 2.0;
                                                parse_quote! { #new_float }
                                            }
                                            _ => default_expr.clone(),
                                        }
                                    }
                                    _ => default_expr.clone(),
                                };
                                new_expr
                            }
                            None => {
                                let new_expr = parse_quote! { 42i32 };
                                new_expr
                            }
                        };
                        const_param.default = Some(new_expr);
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                let new_expr = match const_param.default {
                                    Some(ref default_expr) => {
                                        let new_expr = match default_expr {
                                            Expr::Lit(lit_expr) => {
                                                let lit = &lit_expr.lit;
                                                match lit {
                                                    Lit::Int(int_lit) => {
                                                        let new_int = int_lit.base10_parse::<i32>().unwrap() * 2;
                                                        parse_quote! { #new_int }
                                                    }
                                                    Lit::Float(float_lit) => {
                                                        let new_float = float_lit.base10_parse::<f64>().unwrap() * 2.0;
                                                        parse_quote! { #new_float }
                                                    }
                                                    _ => default_expr.clone(),
                                                }
                                            }
                                            _ => default_expr.clone(),
                                        };
                                        new_expr
                                    }
                                    None => {
                                        let new_expr = parse_quote! { 42i32 };
                                        new_expr
                                    }
                                };
                                const_param.default = Some(new_expr);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &'static str {
        "The mutation operator replaces constant expressions used in generic parameters with new constant expressions. This transformation tests the compiler's ability to handle complex constant expressions in generic parameters, which is relevant to the bug report labeled A-const-generics."
    }
}