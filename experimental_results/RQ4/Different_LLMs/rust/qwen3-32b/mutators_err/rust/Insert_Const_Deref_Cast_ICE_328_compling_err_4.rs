pub struct Insert_Const_Deref_Cast_ICE_328;

impl Mutator for Insert_Const_Deref_Cast_ICE_328 {
    fn name(&self) -> &str {
        "Insert_Const_Deref_Cast_ICE_328"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(_) = &*pat_type.ty {
                                let param_name = match &*pat_type.pat {
                                    Pat::Ident(ident) => ident.ident.clone(),
                                    _ => continue,
                                };
                                let new_expr: Expr = parse_quote! {
                                    let _y = &*#param_name as &str;
                                };
                                if let Some(block) = func.block.as_mut() {
                                    block.stmts.push(Stmt::Expr(new_expr, None));
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}