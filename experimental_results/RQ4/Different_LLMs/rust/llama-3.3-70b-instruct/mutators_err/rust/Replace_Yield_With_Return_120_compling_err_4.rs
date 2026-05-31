use syn::{File, Item, ItemFn, Block, Stmt, Expr, ExprYield, ExprReturn};
use crate::mutator::Mutator;

struct Replace_Yield_With_Return_120;

impl Mutator for Replace_Yield_With_Return_120 {
    fn name(&self) -> &str {
        "Replace_Yield_With_Return_120"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut new_block = func.block.clone();
                new_block.stmts = new_block.stmts.into_iter().map(|stmt| {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Yield(yield_expr) = expr {
                            Stmt::Expr(Expr::Return(ExprReturn {
                                expr: yield_expr.expr.clone(),
                                attrs: vec![],
                                from: None,
                            }), vec![])
                        } else {
                            Stmt::Expr(expr, vec![])
                        }
                    } else {
                        stmt
                    }
                }).collect();
                func.block = new_block;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that use the yield keyword, typically found in generator functions or coroutines. It replaces the yield statement with a return statement, potentially altering the control flow and return type of the function."
    }
}