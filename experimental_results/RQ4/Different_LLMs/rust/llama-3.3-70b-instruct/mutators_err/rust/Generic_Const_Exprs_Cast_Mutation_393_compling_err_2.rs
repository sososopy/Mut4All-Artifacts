impl Mutator for Generic_Const_Exprs_Cast_Mutation_393 {
    fn name(&self) -> &str {
        "Generic_Const_Exprs_Cast_Mutation_393"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for statement in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = statement {
                                if let Expr::Cast(expr_cast) = expr {
                                    if let Type::Path(type_path) = &*expr_cast.expr {
                                        let new_type = match type_path.path.segments.last() {
                                            Some(segment) => {
                                                let new_ident = match segment.ident.to_string().as_str() {
                                                    "f64" => "u128",
                                                    "u128" => "f64",
                                                    _ => &segment.ident.to_string(),
                                                };
                                                Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath {
                                                        leading_colon: None,
                                                        segments: Punctuated::from_iter(vec![PathSegment {
                                                            ident: Ident::new(new_ident, Span::call_site()),
                                                            arguments: PathArguments::None,
                                                        }]),
                                                    },
                                                })
                                            }
                                            None => continue,
                                        };
                                        expr_cast.ty = Box::new(new_type);
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
        "The mutation operator targets Rust code that uses generic const expressions, specifically focusing on constant casts within trait implementations or generic types. It introduces subtle errors by modifying the type of constants or changing the casting operations to trigger bugs in the Rust compiler's handling of generic const expressions."
    }
}