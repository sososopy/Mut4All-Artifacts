use crate::mutator::Mutator;
use syn::{Expr, Stmt, parse_quote, visit_mut::VisitMut};

pub struct Alter_Loop_Break_Value_18;

impl Mutator for Alter_Loop_Break_Value_18 {
    fn name(&self) -> &str {
        "Alter_Loop_Break_Value_18"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct LoopVisitor;

        impl VisitMut for LoopVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Block(expr_block) = expr {
                    if expr_block.attrs.iter().any(|attr| attr.path().is_ident("const")) {
                        for stmt in &mut expr_block.block.stmts {
                            if let Stmt::Expr(expr_stmt, _) = stmt {
                                if let Expr::While(expr_while) = expr_stmt {
                                    let stmts = &mut expr_while.body.stmts;
                                    if let Some(break_expr) = stmts.iter_mut()
                                        .filter_map(|stmt| if let Stmt::Expr(Expr::Break(expr_break), _) = stmt {
                                            Some(expr_break)
                                        } else {
                                            None
                                        }).next() {
                                            if let Some(_) = break_expr.expr {
                                                break_expr.expr = Some(Box::new(Expr::Tuple(parse_quote!(()))));
                                            }
                                    }
                                } else if let Expr::Loop(expr_loop) = expr_stmt {
                                    let stmts = &mut expr_loop.body.stmts;
                                    if let Some(break_expr) = stmts.iter_mut()
                                        .filter_map(|stmt| if let Stmt::Expr(Expr::Break(expr_break), _) = stmt {
                                            Some(expr_break)
                                        } else {
                                            None
                                        }).next() {
                                            if let Some(_) = break_expr.expr {
                                                break_expr.expr = Some(Box::new(Expr::Tuple(parse_quote!(()))));
                                            }
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = LoopVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies loops within constant evaluation contexts, specifically targeting 'while' or 'loop' constructs. It locates 'break' statements within these loops and alters the value returned by the 'break' statement to a unit type '()', aiming to induce type-related issues during const evaluation."
    }
}