use syn::{File, Item, ItemConst, Expr, ExprUnary, UnOp};
use syn::token::{Minus, Not, Star};
use rand::Rng;
use crate::mutator::Mutator;

struct Replace_Dereference_Operator_52;

impl Mutator for Replace_Dereference_Operator_52 {
    fn name(&self) -> &str {
        "Replace_Dereference_Operator_52"
    }

    fn chain_of_thought(&self) -> &str {
        "Replace dereference operator with negation, logical NOT, or dereference operator"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Expr::Unary(unary_expr) = &*const_item.expr {
                    if let UnOp::Deref(_) = &unary_expr.op {
                        let new_op = match rand::thread_rng().gen_range(0..3) {
                            0 => UnOp::Neg(Minus::default()),
                            1 => UnOp::Not(Not::default()),
                            2 => UnOp::Deref(Star::default()),
                            _ => unreachable!(),
                        };
                        let new_unary_expr = Expr::Unary(ExprUnary {
                            attrs: unary_expr.attrs.clone(),
                            op: new_op,
                            expr: Box::new(*unary_expr.expr.clone()),
                        });
                        const_item.expr = Box::new(new_unary_expr);
                    }
                }
            }
        }
    }
}