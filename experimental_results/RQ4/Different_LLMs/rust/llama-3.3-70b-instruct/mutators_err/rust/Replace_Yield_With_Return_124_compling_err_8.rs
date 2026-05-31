use syn::{parse_quote, Expr, Stmt};
use crate::mutator::Mutator;

struct Replace_Yield_With_Return_124;

impl Mutator for Replace_Yield_With_Return_124 {
    fn name(&self) -> &str {
        "Replace_Yield_With_Return_124"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Yield(yield_expr) = expr {
                            let return_expr = parse_quote! {
                                return #yield_expr.expr;
                            };
                            *expr = return_expr;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces yield statements with return statements in functions, effectively changing the behavior of the function from producing a series of values to producing a single value. This transformation tests the compiler's handling of coroutines and iterators, specifically the yield keyword."
    }
}