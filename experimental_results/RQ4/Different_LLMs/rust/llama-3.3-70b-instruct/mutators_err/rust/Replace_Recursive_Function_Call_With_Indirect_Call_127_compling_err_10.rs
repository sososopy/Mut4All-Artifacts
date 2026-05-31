use quote::quote;
use syn::{parse_quote, ReturnType, Type, Stmt, Expr};
use crate::mutator::Mutator;

struct Replace_Recursive_Function_Call_With_Indirect_Call_127;

impl Mutator for Replace_Recursive_Function_Call_With_Indirect_Call_127 {
    fn name(&self) -> &str {
        "Replace_Recursive_Function_Call_With_Indirect_Call_127"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(_) = **return_type {
                        let func_ident = &func.sig.ident;
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Call(call_expr) = expr {
                                    if let Expr::Path(path_expr) = &*call_expr.func {
                                        if path_expr.path.segments.len() > 0 && path_expr.path.segments[0].ident == *func_ident {
                                            // Replace the recursive call with an indirect call
                                            let indirect_call = quote! {
                                                let f = #func_ident;
                                                f()
                                            };
                                            *expr = parse_quote! { #indirect_call };
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
        "The mutation operator targets recursive functions that return an implementation of a trait using the 'impl Trait' syntax. It replaces the direct recursive call with an indirect call through a function pointer or a closure. This transformation can help expose issues with how the compiler handles the 'impl Trait' syntax in conjunction with recursive functions and destructors."
    }
}