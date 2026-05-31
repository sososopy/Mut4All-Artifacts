impl Mutator for Change_Constant_Value_347 {
    fn name(&self) -> &str {
        "Change_Constant_Value_347"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Expr::Lit(ExprLit {
                    lit: Lit::Int(int_lit),
                    ..
                }) = &*const_item.expr {
                    let new_value = thread_rng().gen_range(0..100);
                    const_item.expr = parse_quote! { #new_value };
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The Change_Constant_Value_347 mutation operator changes the value of constants in a program. It aims to test the compiler's ability to handle different constant values and detect potential errors."
    }
}