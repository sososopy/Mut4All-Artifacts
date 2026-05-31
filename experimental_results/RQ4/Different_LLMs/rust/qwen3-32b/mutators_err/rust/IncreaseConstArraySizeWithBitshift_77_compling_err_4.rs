pub struct IncreaseConstArraySizeWithBitshift_77;

impl Mutator for IncreaseConstArraySizeWithBitshift_77 {
    fn name(&self) -> &str {
        "IncreaseConstArraySizeWithBitshift_77"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Some(expr_box) = item_const.expr.as_mut() {
                    let mut visitor = ArraySizeMutator;
                    visitor.visit_expr_mut(&mut *expr_box);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator increases the size of array initializers in const declarations by replacing their size expressions with 'usize::MAX >> 16'. This transformation stresses the compiler's const evaluation system by requesting a large array allocation during compilation, potentially triggering ICEs in the const evaluator due to excessive size."
    }
}

struct ArraySizeMutator;

impl VisitMut for ArraySizeMutator {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Repeat(expr_repeat) = expr {
            expr_repeat.len = Box::new(parse_quote! { usize::MAX >> 16 });
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}