impl Mutator for Replace_Character_Literal_With_String_Literal_233 {
    fn name(&self) -> &str {
        "Replace_Character_Literal_With_String_Literal_233"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Lit(lit) = expr {
                            if let Lit::Char(c) = &lit.lit {
                                let string_literal = Lit::Str(LitStr::new(&format!("{:?}", c), lit.span()));
                                *expr = parse_quote! { #string_literal };
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Lit(lit) = expr {
                                    if let Lit::Char(c) = &lit.lit {
                                        let string_literal = Lit::Str(LitStr::new(&format!("{:?}", c), lit.span()));
                                        *expr = parse_quote! { #string_literal };
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
        "The mutation operator replaces character literals with string literals of the same character. This transformation tests the compiler's ability to handle Unicode and generic types, potentially triggering type mismatches or inference failures."
    }
}