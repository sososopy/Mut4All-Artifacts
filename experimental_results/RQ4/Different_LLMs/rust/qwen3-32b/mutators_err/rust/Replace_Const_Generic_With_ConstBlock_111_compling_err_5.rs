pub struct Replace_Const_Generic_With_ConstBlock_111;

impl Mutator for Replace_Const_Generic_With_ConstBlock_111 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_ConstBlock_111"
    }
    fn mutate(&self, file: &mut syn::File) {
        let const_vars: Vec<Ident> = file.items.iter()
            .filter_map(|item| {
                if let Item::Const(item_const) = item {
                    if let Type::Infer(_) = &*item_const.ty {
                        Some(item_const.ident.clone())
                    } else {
                        None
                    }
                } else {
                    None
                }
            })
            .collect();

        if const_vars.is_empty() {
            return;
        }

        struct CallVisitor {
            const_vars: Vec<Ident>,
        }

        impl VisitMut for CallVisitor {
            fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
                if let Expr::Path(func_path) = &*call.func {
                    if let Some(last_segment) = func_path.path.segments.last_mut() {
                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    let ident = &self.const_vars[0];
                                    let expr_path = Expr::Path(ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: syn::Path::from(ident.clone()),
                                    });
                                    let block = ExprBlock {
                                        attrs: vec![],
                                        label: None,
                                        block: parse_quote!({ #expr_path }),
                                    };
                                    *arg = GenericArgument::Const(Box::new(Expr::Block(block)));
                                }
                            }
                        }
                    }
                }
                visit_mut::visit_expr_call_mut(self, call);
            }
        }

        let mut visitor = CallVisitor { const_vars };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}