use syn::{File, Item, GenericParam, Expr, Lit, LitInt, LitFloat, ExprLit, Data, Generics, ItemStruct};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Substitute_Const_Generic_Defaults_87;

impl Mutator for Substitute_Const_Generic_Defaults_87 {
    fn name(&self) -> &str {
        "Substitute_Const_Generic_Defaults_87"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Struct(struct_def) = item {
                if let Generics { params, .. } = &mut struct_def.generics {
                    for param in params {
                        if let GenericParam::Const(param) = param {
                            if let Some(default) = &param.default {
                                let new_default = match default {
                                    Expr::Lit(lit) => {
                                        let new_value = match &lit.lit {
                                            Lit::Int(int) => {
                                                let new_value = int.base10_parse::<u64>().unwrap() + 1;
                                                Lit::Int(LitInt::new(&new_value.to_string(), Span::call_site()))
                                            }
                                            Lit::Float(float) => {
                                                let new_value = float.base10_parse::<f64>().unwrap() + 1.0;
                                                Lit::Float(LitFloat::new(&new_value.to_string(), Span::call_site()))
                                            }
                                            _ => panic!("Unsupported literal type"),
                                        };
                                        Expr::Lit(ExprLit {
                                            lit: new_value,
                                            attrs: vec![],
                                        })
                                    }
                                    _ => panic!("Unsupported default expression"),
                                };
                                param.default = Some(new_default);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes the default values of const generics in struct definitions. It aims to test the compiler's handling of const generics with different default values and their interaction with other language features."
    }
}