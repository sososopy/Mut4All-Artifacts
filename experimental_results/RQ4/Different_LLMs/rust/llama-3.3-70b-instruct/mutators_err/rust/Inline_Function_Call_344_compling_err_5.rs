use syn::{Item, Stmt, Expr, Block};
use crate::mutator::Mutator;

struct Inline_Function_Call_344;

impl Mutator for Inline_Function_Call_344 {
    fn name(&self) -> &str {
        "Inline_Function_Call_344"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Call(expr_call) = expr {
                            if let Expr::Path(expr_path) = &*expr_call.func {
                                if let Some(inline_attr) = expr_path.path.segments.last() {
                                    if inline_attr.ident == "inline" {
                                        // Find the function definition
                                        let func_def = file.items.iter().find_map(|item| {
                                            if let Item::Fn(func) = item {
                                                if func.sig.ident == inline_attr.ident {
                                                    Some(func)
                                                } else {
                                                    None
                                                }
                                            } else {
                                                None
                                            }
                                        });

                                        if let Some(func_def) = func_def {
                                            // Inline the function call
                                            let mut new_block = Block {
                                                brace_token: Default::default(),
                                                stmts: func_def.block.stmts.clone(),
                                            };

                                            // Replace the function call with the inlined block
                                            *stmt = Stmt::Block(new_block);
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
        "The mutation operator replaces function calls with their inlined versions. It targets functions marked with the inline attribute and copies their bodies to replace the function calls. This transformation aims to test the compiler's handling of inlined functions, const evaluation, and potential ICEs related to the inline attribute."
    }
}