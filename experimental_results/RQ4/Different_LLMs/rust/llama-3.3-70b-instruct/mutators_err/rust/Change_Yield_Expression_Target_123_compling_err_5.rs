use syn::{parse_quote, Expr, Stmt};
use quote::quote;
use crate::mutator::Mutator;

struct Change_Yield_Expression_Target_123;

impl Mutator for Change_Yield_Expression_Target_123 {
    fn name(&self) -> &str {
        "Change_Yield_Expression_Target_123"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Yield(yield_expr) = expr {
                            let new_yield_expr = match yield_expr.expr.as_ref() {
                                Some(Expr::Lit(lit)) => {
                                    quote! { Some(#lit) }
                                }
                                Some(Expr::Path(path)) => {
                                    quote! { (#path, 6) }
                                }
                                Some(expr) => {
                                    quote! { (#expr, 7) }
                                }
                                None => {
                                    quote! { ((), 7) }
                                }
                            };
                            *expr = parse_quote! { yield #new_yield_expr };
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the target of a yield expression with a different type or value. This transformation helps expose bugs related to coroutines and generator functionality in the Rust compiler."
    }
}