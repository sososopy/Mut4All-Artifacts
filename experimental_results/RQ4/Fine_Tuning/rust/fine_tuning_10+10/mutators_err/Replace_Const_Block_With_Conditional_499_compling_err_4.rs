use crate::mutator::Mutator;
use syn::{Expr, Stmt};

pub struct Replace_Const_Block_With_Conditional_499;

impl Mutator for Replace_Const_Block_With_Conditional_499 {
    fn name(&self) -> &str {
        "Replace_Const_Block_With_Conditional_499"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    let mut new_stmts = Vec::new();
                    for stmt in &func.block.stmts {
                        if let Stmt::Expr(Expr::Block(expr_block)) = stmt {
                            if expr_block.block.stmts.len() == 1 {
                                if let Stmt::Expr(Expr::If(expr_if)) = &expr_block.block.stmts[0] {
                                    new_stmts.push(Stmt::Expr(Expr::If(expr_if.clone())));
                                    continue;
                                }
                            }
                        }
                        new_stmts.push(stmt.clone());
                    }
                    func.block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies constant blocks within `const fn` functions and replaces them with inline conditional expressions. This transformation removes the explicit `const` block and directly uses the conditional logic, aiming to test how the compiler handles constant evaluation and optimization in constant functions. It challenges the compiler's ability to process inline expressions and may uncover issues related to constant folding or evaluation."
    }
}