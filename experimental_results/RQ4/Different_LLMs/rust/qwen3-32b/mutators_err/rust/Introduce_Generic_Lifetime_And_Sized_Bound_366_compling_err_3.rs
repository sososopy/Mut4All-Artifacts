pub struct Introduce_Generic_Lifetime_And_Sized_Bound_366;

impl Mutator for Introduce_Generic_Lifetime_And_Sized_Bound_366 {
    fn name(&self) -> &str {
        "Introduce_Generic_Lifetime_And_Sized_Bound_366"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_const_fns = HashSet::new();

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    if let ReturnType::Type(_, ty) = &mut func.sig.output {
                        if let Type::Path(type_path) = &**ty {
                            if type_path.path.is_ident("usize") {
                                let lifetime = parse_quote!('a);
                                func.sig.generics.params.push(GenericParam::Lifetime(lifetime));
                                let where_clause = func.sig.generics.where_clause.get_or_insert_with(|| {
                                    WhereClause {
                                        where_token: token::Where::default(),
                                        predicates: Punctuated::new(),
                                    }
                                });
                                where_clause.predicates.push(parse_quote! { &'a (): Sized });
                                modified_const_fns.insert(func.sig.ident.to_string());
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Type(pat_type) = &mut local.pat {
                            let ty = &mut *pat_type.ty;
                            if let Type::Array(array_type) = &mut **ty {
                                if let Expr::Call(call_expr) = &mut array_type.len {
                                    if let Expr::Path(expr_path) = &mut *call_expr.func {
                                        let func_name = expr_path.path.segments.first().unwrap().ident.to_string();
                                        if modified_const_fns.contains(&func_name) {
                                            let lifetime = parse_quote!('a);
                                            func.sig.generics.params.push(GenericParam::Lifetime(lifetime));
                                            let ident = expr_path.path.segments.first().unwrap().ident.clone();
                                            let new_path = parse_quote!(#ident::<'a>);
                                            call_expr.func = Box::new(Expr::Path(ExprPath {
                                                attrs: Vec::new(),
                                                qself: None,
                                                path: new_path,
                                            }));
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
        ""
    }
}