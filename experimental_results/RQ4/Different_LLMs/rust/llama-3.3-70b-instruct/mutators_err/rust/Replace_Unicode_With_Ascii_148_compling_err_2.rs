impl Mutator for Replace_Unicode_With_Ascii_148 {
    fn name(&self) -> &str {
        "Replace_Unicode_With_Ascii_148"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call) = expr {
                            if let Expr::Path(path) = &*call.func {
                                let mut new_path = path.clone();
                                new_path.path.segments.iter_mut().for_each(|segment| {
                                    if let Some(ident) = segment.ident.to_string().chars().next() {
                                        if ident.is_ascii() == false {
                                            segment.ident = Ident::new(&ident.to_string().to_ascii_lowercase(), segment.ident.span());
                                        }
                                    }
                                });
                                call.func = Box::new(Expr::Path(new_path));
                            }
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Call(call) = expr {
                                    if let Expr::Path(path) = &*call.func {
                                        let mut new_path = path.clone();
                                        new_path.path.segments.iter_mut().for_each(|segment| {
                                            if let Some(ident) = segment.ident.to_string().chars().next() {
                                                if ident.is_ascii() == false {
                                                    segment.ident = Ident::new(&ident.to_string().to_ascii_lowercase(), segment.ident.span());
                                                }
                                            }
                                        });
                                        call.func = Box::new(Expr::Path(new_path));
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
        "The mutation operator replaces Unicode characters with their ASCII counterparts in function and method calls. This transformation tests the compiler's handling of Unicode characters and its ability to distinguish between similar-looking characters."
    }
}