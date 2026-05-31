use syn::{parse_quote, Expr, ExprAsync, Stmt};
use crate::mutator::Mutator;

struct Replace_Async_Block_With_Await_119;

impl Mutator for Replace_Async_Block_With_Await_119 {
    fn name(&self) -> &str {
        "Replace_Async_Block_With_Await_119"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(block) = &mut func.block {
                    for stmt in block.stmts.iter_mut() {
                        if let Stmt::Expr(expr) = stmt {
                            if let Expr::Async(ExprAsync {
                                async_token,
                                capture,
                                block: async_block,
                                ..
                            }) = expr {
                                let new_stmt = Stmt::Expr(
                                    parse_quote! {
                                        {
                                            #async_block
                                            let _ = async { }.await;
                                        }
                                    },
                                );
                                *stmt = new_stmt;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets asynchronous blocks within functions and replaces them with similar blocks containing an additional await expression. This transformation introduces variations in asynchronous code, potentially exposing issues related to monomorphization and ICE in the Rust compiler."
    }
}