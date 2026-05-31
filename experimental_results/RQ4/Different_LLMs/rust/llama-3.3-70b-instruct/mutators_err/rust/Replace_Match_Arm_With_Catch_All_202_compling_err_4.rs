use syn::{parse_quote, Item, Stmt, Expr};
use rand::thread_rng;
use crate::mutator::Mutator;

struct Replace_Match_Arm_With_Catch_All_202;

impl Mutator for Replace_Match_Arm_With_Catch_All_202 {
    fn name(&self) -> &str {
        "Replace_Match_Arm_With_Catch_All_202"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Match(expr_match) = expr {
                            if let Some((index, _)) = expr_match.arms.iter().enumerate().choose(&mut thread_rng()) {
                                expr_match.arms[index] = parse_quote! { _ => () };
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a randomly chosen match arm with a catch-all arm (_). This transformation tests the compiler's handling of pattern matching and may lead to unexpected behavior or errors if the replaced arm was essential to the program's logic."
    }
}