use rand::thread_rng;
use syn::{Item, GenericParam, Ident, ExprCall, ExprPath, SynPath, PathSegment, PathArguments, punctuated::Punctuated, Expr};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Replace_Const_Generic_Param_With_Function_Call_293;

impl Mutator for Replace_Const_Generic_Param_With_Function_Call_293 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Param_With_Function_Call_293"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            if let Some(default) = &mut param.default {
                                let func_name = Ident::new(&format!("some_function_{}", thread_rng().gen::<u32>()), Span::call_site());
                                let func_call = ExprCall {
                                    func: ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: func_name,
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    },
                                    paren_token: syn::token::Paren {
                                        span: Span::call_site(),
                                    },
                                    args: Punctuated::new(),
                                };
                                *default = Box::new(Expr::Call(func_call));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters in struct definitions with function calls. This transformation aims to test the compiler's handling of const generics and function calls, potentially triggering bugs related to const generic parameters."
    }
}