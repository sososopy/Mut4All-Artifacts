pub struct Replace_Const_Param_With_ArithExpr_89;

impl Mutator for Replace_Const_Param_With_ArithExpr_89 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_ArithExpr_89"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(pred) = predicate {
                            if let syn::Type::Path(type_path) = &mut pred.bounded_ty {
                                for segment in &mut type_path.path.segments {
                                    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let syn::GenericArgument::Const(expr) = arg {
                                                let new_expr = create_arithmetic_expr_add_one(&Box::new(expr.clone()));
                                                *arg = syn::GenericArgument::Const(*new_expr);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(pred) = predicate {
                                    if let syn::Type::Path(type_path) = &mut pred.bounded_ty {
                                        for segment in &mut type_path.path.segments {
                                            if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                for arg in &mut args.args {
                                                    if let syn::GenericArgument::Const(expr) = arg {
                                                        let new_expr = create_arithmetic_expr_add_one(&Box::new(expr.clone()));
                                                        *arg = syn::GenericArgument::Const(*new_expr);
                                                    }
                                                }
                                            }
                                        }
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
        "The mutation operator replaces const parameters in trait bounds within where clauses with arithmetic expressions (e.g., N → N + 1). This transformation creates unsatisfied trait bounds by altering const values, testing the compiler's ability to detect and report such mismatches, potentially exposing ICEs in const evaluation or trait resolution."
    }
}

fn create_arithmetic_expr_add_one(expr: &Box<syn::Expr>) -> Box<syn::Expr> {
    let one = syn::Expr::Lit(syn::ExprLit {
        attrs: vec![],
        lit: syn::Lit::Int(syn::LitInt::new("1", Span::call_site())),
    });
    let op = syn::BinOp::Add(syn::token::Plus::ne(Span::call_site()));
    Box::new(syn::Expr::Binary(syn::ExprBinary {
        attrs: vec![],
        left: expr.clone(),
        op,
        right: Box::new(one),
    }))
}