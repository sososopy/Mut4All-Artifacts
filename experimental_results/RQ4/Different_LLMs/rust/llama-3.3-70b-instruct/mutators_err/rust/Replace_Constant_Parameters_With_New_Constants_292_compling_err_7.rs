use syn::{File, Item, ItemFn, ItemImpl, ImplItem, ImplItemFn, GenericParam, Expr, Lit, LitStr, LitInt, LitBool, ExprLit};
use syn::visit_mut::VisitMut;

struct Replace_Constant_Parameters_With_New_Constants_292;

impl Replace_Constant_Parameters_With_New_Constants_292 {
    fn name(&self) -> &str {
        "Replace_Constant_Parameters_With_New_Constants_292"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(param) = param {
                        let new_const_value = match &param.default {
                            Some(expr) => {
                                match expr {
                                    Expr::Lit(lit) => {
                                        match lit.lit {
                                            Lit::Str(str) => {
                                                // Replace string constant with a new string constant
                                                Expr::Lit(ExprLit {
                                                    attrs: vec![],
                                                    lit: Lit::Str(LitStr::new(
                                                        &format!("{}_new", str.value()),
                                                        str.span(),
                                                    )),
                                                })
                                            }
                                            Lit::Int(int) => {
                                                // Replace integer constant with a new integer constant
                                                Expr::Lit(ExprLit {
                                                    attrs: vec![],
                                                    lit: Lit::Int(LitInt::new(
                                                        &format!("{}", int.base10_parse::<i32>().unwrap() + 1),
                                                        int.span(),
                                                    )),
                                                })
                                            }
                                            Lit::Bool(bool) => {
                                                // Replace boolean constant with a new boolean constant
                                                Expr::Lit(ExprLit {
                                                    attrs: vec![],
                                                    lit: Lit::Bool(LitBool::new(!bool.value, bool.span())),
                                                })
                                            }
                                            _ => panic!("Unsupported constant type"),
                                        }
                                    }
                                    _ => panic!("Unsupported expression type"),
                                }
                            }
                            None => panic!("No default value provided"),
                        };
                        param.default = Some(new_const_value);
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(param) = param {
                                let new_const_value = match &param.default {
                                    Some(expr) => {
                                        match expr {
                                            Expr::Lit(lit) => {
                                                match lit.lit {
                                                    Lit::Str(str) => {
                                                        // Replace string constant with a new string constant
                                                        Expr::Lit(ExprLit {
                                                            attrs: vec![],
                                                            lit: Lit::Str(LitStr::new(
                                                                &format!("{}_new", str.value()),
                                                                str.span(),
                                                            )),
                                                        })
                                                    }
                                                    Lit::Int(int) => {
                                                        // Replace integer constant with a new integer constant
                                                        Expr::Lit(ExprLit {
                                                            attrs: vec![],
                                                            lit: Lit::Int(LitInt::new(
                                                                &format!("{}", int.base10_parse::<i32>().unwrap() + 1),
                                                                int.span(),
                                                            )),
                                                        })
                                                    }
                                                    Lit::Bool(bool) => {
                                                        // Replace boolean constant with a new boolean constant
                                                        Expr::Lit(ExprLit {
                                                            attrs: vec![],
                                                            lit: Lit::Bool(LitBool::new(!bool.value, bool.span())),
                                                        })
                                                    }
                                                    _ => panic!("Unsupported constant type"),
                                                }
                                            }
                                            _ => panic!("Unsupported expression type"),
                                        }
                                    }
                                    None => panic!("No default value provided"),
                                };
                                param.default = Some(new_const_value);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant parameters with new constant values or expressions, potentially triggering errors if the compiler does not handle these new constants correctly. It aims to achieve high coverage across different program structures and features, particularly focusing on constant expressions, generic types, and trait implementations."
    }
}