impl Mutator for Replace_Enum_Variant_With_Invalid_Pattern_238 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_With_Invalid_Pattern_238"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Expr::Block(block) = &mut *func.block {
                    for stmt in &mut block.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Match(match_expr) = expr {
                                for arm in &mut match_expr.arms {
                                    if let Pat::Path(path) = &arm.pat {
                                        let invalid_pattern = parse_quote! { Some((a, b)) };
                                        arm.pat = invalid_pattern;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces enum variants in match statements with an invalid pattern. It applies to code constructs where an enum variant is matched against a pattern that does not match its type. The goal is to trigger bugs related to exhaustiveness checking and pattern matching."
    }
}

impl Replace_Enum_Variant_With_Invalid_Pattern_238 {
    pub fn new() -> Self {
        Replace_Enum_Variant_With_Invalid_Pattern_238
    }
}