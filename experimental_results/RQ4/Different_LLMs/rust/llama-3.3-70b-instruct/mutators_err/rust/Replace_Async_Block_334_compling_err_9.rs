use crate::mutator::Mutator;
use rand::thread_rng;
use rand::Rng;
use syn::{Block, Expr, ExprAsync, Item, Stmt, token};

struct Replace_Async_Block_334;

impl Mutator for Replace_Async_Block_334 {
    fn name(&self) -> &str {
        "Replace_Async_Block_334"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut expressions = Vec::new();
        for item in &file.items {
            if let Item::Fn(func) = item {
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        expressions.push(expr.clone());
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Async(ExprAsync { block, .. }) = expr {
                            if let Some(replacement) = expressions.choose(&mut thread_rng()) {
                                let new_block = Block {
                                    brace_token: block.brace_token,
                                    stmts: {
                                        let mut stmts: Vec<Stmt> = vec![];
                                        stmts.push(Stmt::Expr(replacement.clone(), None));
                                        stmts
                                    },
                                };
                                *expr = Expr::Async(ExprAsync {
                                    attrs: vec![],
                                    async_token: token::Async { span: proc_macro2::Span::call_site() },
                                    capture: None,
                                    block: new_block,
                                });
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the content of async blocks with a randomly chosen expression from the same function or module. This transformation aims to test the compiler's handling of async blocks and their interaction with other language features, potentially triggering bugs related to async block handling."
    }
}