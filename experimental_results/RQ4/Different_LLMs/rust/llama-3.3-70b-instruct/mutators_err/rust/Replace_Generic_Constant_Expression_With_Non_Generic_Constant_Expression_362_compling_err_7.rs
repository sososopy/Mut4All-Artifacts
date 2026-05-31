impl Replace_Generic_Constant_Expression_With_Non_Generic_Constant_Expression_362 {
    fn mutate_arg(&self, arg: &mut GenericArgument) {
        if let GenericArgument::Const(Expr::Binary(ExprBinary {
            op: BinOp::Add(_),
            left: Expr::Const(ExprConst {
                attrs: _,
                const_token: _,
                block,
            }),
            right: Expr::Const(ExprConst {
                attrs: _,
                const_token: _,
                block: _,
            }),
        })) = arg {
            *arg = GenericArgument::Const(Expr::Lit(ExprLit {
                attrs: Vec::new(),
                lit: Lit::Int(LitInt {
                    ..Default::default()
                }),
            }));
        }
    }
}

impl Mutator for Replace_Generic_Constant_Expression_With_Non_Generic_Constant_Expression_362 {
    fn name(&self) -> &str {
        "Replace_Generic_Constant_Expression_With_Non_Generic_Constant_Expression_362"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(item_type) = impl_item {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }) = item_type.ty {
                            for segment in segments {
                                if let PathSegment {
                                    ident,
                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: _,
                                        args,
                                        gt_token: _,
                                    }),
                                } = segment {
                                    for arg in args {
                                        self.mutate_arg(arg);
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
        "The mutation operator replaces generic constant expressions with non-generic constant expressions in type aliases within impl blocks. It aims to test the compiler's handling of constant expressions and type aliases, potentially triggering bugs related to type checking or code generation."
    }
}