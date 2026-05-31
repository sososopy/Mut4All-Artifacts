```rust
impl Mutator for Replace_Dereference_Operator_52 {
    fn name(&self) -> &str {
        "Replace_Dereference_Operator_52"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let Expr::Unary(unary_expr) = &*const_item.expr {
                    if let syn::UnOp::Deref(_) = unary_expr.op {
                        let new_op = match thread_rng().gen_range(0..3) {
 => syn::UnOp::Neg(token::Minus),
                            1 => syn::UnOp::Not(token::Not),