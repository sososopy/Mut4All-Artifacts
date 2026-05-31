impl Mutator for Modify_ArrayIndex_With_Offset_219 {
    fn name(&self) -> &str {
        "Modify_ArrayIndex_With_Offset_219"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Index(expr_index) = &**expr {
                            let index = &expr_index.index;
                            if let Expr::Lit(ExprLit {
                                lit: Lit::Int(int_lit),
                                ..
                            }) = &**index {
                                let offset = if thread_rng().gen_bool(0.5) {
                                    1
                                } else {
                                    -1
                                };
                                let new_index = parse_quote! { #int_lit + #offset };
                                expr_index.index = Box::new(new_index);
                            } else if let Expr::Path(path) = &**index {
                                let offset = if thread_rng().gen_bool(0.5) {
                                    1
                                } else {
                                    -1
                                };
                                let new_index = parse_quote! { #path + #offset };
                                expr_index.index = Box::new(new_index);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies array or slice indexing operations by offsetting the index value. It randomly chooses to either increment or decrement the index, potentially exposing bugs related to bounds checking and indexing."
    }
}