use crate::mutator::Mutator;
use syn::{parse_quote, Expr, ExprCall, ExprPath, Item, Stmt};

pub struct Modify_Closure_Invocation_And_Output_441;

impl Mutator for Modify_Closure_Invocation_And_Output_441 {
    fn name(&self) -> &str {
        "Modify_Closure_Invocation_And_Output_441"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut has_target_trait = false;
                let mut closure_invocation: Option<Expr> = None;

                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.iter().any(|seg| seg.ident == "Fn" || seg.ident == "FnOnce") {
                        has_target_trait = true;
                    }
                }

                if has_target_trait {
                    for item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(method) = item {
                            let mut target_stmt_index: Option<usize> = None;
                            for (i, stmt) in method.block.stmts.iter_mut().enumerate() {
                                if let Stmt::Expr(expr, _) = stmt {
                                    if let Expr::Call(ExprCall { func, .. }) = expr {
                                        if let Expr::Path(ExprPath { path, .. }) = &**func {
                                            if path.segments.iter().any(|seg| seg.ident == "self") {
                                                closure_invocation = Some(expr.clone());
                                                target_stmt_index = Some(i);
                                            }
                                        }
                                    }
                                }
                            }

                            if let Some(expr) = closure_invocation.take() {
                                if let Some(index) = target_stmt_index {
                                    let new_expr: Expr = parse_quote! {
                                        let _ = async { #expr };
                                    };
                                    method.block.stmts[index] = Stmt::Expr(new_expr, None);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait implementations involving unboxed closures. It identifies closure invocations within these implementations and wraps them in an async block, introducing a new async context. This alteration affects the trait's expected output and potentially impacts the compiler's trait selection and monomorphization logic."
    }
}