use syn::{File, Item, ItemFn, Stmt, Expr, ExprCall, ExprPath, ExprClosure, Pat, ReturnType, Token};
use syn::visit_mut::VisitMut;

struct Async_Closure_Argument_Swap_20;

impl Async_Closure_Argument_Swap_20 {
    fn name(&self) -> &str {
        "Async_Closure_Argument_Swap_20"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, _) = &func.sig.output {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Call(expr_call) = expr {
                                if let Expr::Path(expr_path) = &*expr_call.func {
                                    if let Some(segment) = expr_path.path.segments.last() {
                                        if segment.ident.to_string() == "then" {
                                            if let Expr::Closure(closure) = &mut expr_call.args[0] {
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