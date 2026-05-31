impl Mutator for Async_Closure_Argument_Swap_20 {
    fn name(&self) -> &str {
        "Async_Closure_Argument_Swap_20"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, _) = &func.sig.output {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Call(expr_call) = expr {
                                if let Expr::Path(expr_path) = &*expr_call.func {
                                    if let Some(ident) = expr_path.path.segments.last().unwrap().ident.to_string().as_str() {
                                        if ident == "then" {
                                            if let Expr::Closure(closure) = &expr_call.args[0] {
                                                if closure.asyncness.is_some() {
                                                    let mut args: Vec<Pat> = closure.inputs.iter().cloned().collect();
                                                    if args.len() > 1 {
                                                        args.reverse();
                                                        closure.inputs = args.into_iter().collect();
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
        "The mutation operator targets async move closures and swaps the order of arguments within the closure. It applies to async move closures with multiple arguments. This transformation tests the compiler's ability to handle argument ordering within async move closures."
    }
}