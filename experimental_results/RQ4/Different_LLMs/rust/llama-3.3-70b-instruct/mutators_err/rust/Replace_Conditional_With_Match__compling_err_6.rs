use syn::{parse_quote, File, Item, Stmt, Expr};
use rand::thread_rng;
use crate::mutator::Mutator;

struct Replace_Conditional_With_Match_;

impl Mutator for Replace_Conditional_With_Match_ {
    fn mutate(&self, file_ast: &mut File) {
        let mut rng = thread_rng();
        for item in &mut file_ast.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::If(if_expr) = &**expr {
                            let match_arm = parse_quote! {
                                match true {
                                    true => #if_expr.then_branch,
                                    false => #if_expr.else_branch,
                                }
                            };
                            *expr = match_arm;
                        }
                    }
                }
            }
        }
    }

    fn name(&self) -> &str {
        "Replace_Conditional_With_Match_"
    }

    fn chain_of_thought(&self) -> &str {
        "Replace conditional statements with match expressions to potentially induce internal compiler errors"
    }
}

impl Replace_Conditional_With_Match_ {
    fn new() -> Self {
        Replace_Conditional_With_Match_
    }
}

static mut mutator: Replace_Conditional_With_Match_ = Replace_Conditional_With_Match_;