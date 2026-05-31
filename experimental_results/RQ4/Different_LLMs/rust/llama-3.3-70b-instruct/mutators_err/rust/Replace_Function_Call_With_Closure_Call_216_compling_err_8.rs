use syn::{parse_quote, Expr, ExprCall, ExprPath, Stmt};
use crate::mutator::Mutator;

struct Replace_Function_Call_With_Closure_Call_216;

impl Mutator for Replace_Function_Call_With_Closure_Call_216 {
    fn name(&self) -> &str {
        "Replace_Function_Call_With_Closure_Call_216"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(ExprCall { func, .. }) = expr {
                            if let Expr::Path(ExprPath { path, .. }) = **func {
                                let closure_expr: Expr = parse_quote!(|| #path());
                                let closure_call_expr: Expr = parse_quote!(#closure_expr());
                                *expr = closure_call_expr;
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Call(ExprCall { func, .. }) = expr {
                                    if let Expr::Path(ExprPath { path, .. }) = **func {
                                        let closure_expr: Expr = parse_quote!(|| #path());
                                        let closure_call_expr: Expr = parse_quote!(#closure_expr());
                                        *expr = closure_call_expr;
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
        "The mutation operator targets function calls in the code and replaces them with closure calls. This transformation aims to test the compiler's handling of inlining and function calls by introducing a closure call that mimics the behavior of the original function call."
    }
}