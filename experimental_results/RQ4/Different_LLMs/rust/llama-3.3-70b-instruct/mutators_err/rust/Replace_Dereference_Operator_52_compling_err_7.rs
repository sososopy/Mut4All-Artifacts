impl Mutator for Replace_Dereference_Operator_52 {
    fn name(&self) -> &str {
        "Replace_Dereference_Operator_52"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let Expr::Unary(unary_expr) = &*const_item.expr {
                    if let syn::UnOp::Deref(_) = unary_expr.op {
                        let new_op = match rand::thread_rng().gen_range(0..3) {
                            0 => syn::UnOp::Neg(token::Minus),
                            1 => syn::UnOp::Not(token::Not),
                            2 => syn::UnOp::Deref(token::Star),
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