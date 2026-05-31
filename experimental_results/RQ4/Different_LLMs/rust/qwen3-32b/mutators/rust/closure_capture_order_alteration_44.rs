use crate::mutator::Mutator;
use syn::Item;
use syn::Stmt;
use syn::Pat;
use syn::Expr;
use syn::Ident;
use std::collections::{HashMap, HashSet};
use syn::visit::Visit;

pub struct Closure_Capture_Order_Alteration_44;

impl Mutator for Closure_Capture_Order_Alteration_44 {
    fn name(&self) -> &str {
        "Closure_Capture_Order_Alteration_44"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    let block = &mut func.block;
                    let mut new_stmts = vec![];
                    let mut stmts = std::mem::take(&mut block.stmts);
                    let mut local_vars: HashMap<Ident, (usize, Stmt)> = HashMap::new();
                    
                    for (i, stmt) in stmts.iter().enumerate() {
                        if let Stmt::Local(local) = stmt {
                            if let Pat::Ident(pat_ident) = &local.pat {
                                local_vars.insert(pat_ident.ident.clone(), (i, stmt.clone()));
                            }
                        }
                    }

                    for (i, stmt) in stmts.iter().enumerate() {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Return(expr_return) = expr {
                                if let Some(value) = &expr_return.expr {
                                    if let Expr::Closure(closure) = &**value {
                                        let mut collector = MatchVarCollector { vars: HashSet::new() };
                                        collector.visit_expr(&closure.body);
                                        let vars = collector.vars;

                                        let mut to_move = vec![];
                                        for var in &vars {
                                            if let Some((_, stmt)) = local_vars.get(var) {
                                                to_move.push(stmt.clone());
                                            }
                                        }

                                        new_stmts.push(stmt.clone());
                                        for s in to_move {
                                            new_stmts.push(s);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    for s in stmts {
                        new_stmts.push(s);
                    }

                    block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator alters the order of variable captures in closures returned from const functions, placing the closure before the declaration of variables used in match patterns. This disrupts the expected capture order, potentially exposing ICEs during closure type inference or pattern matching resolution."
    }
}

struct MatchVarCollector {
    vars: HashSet<Ident>,
}

impl<'ast> Visit<'ast> for MatchVarCollector {
    fn visit_pat(&mut self, pat: &Pat) {
        if let Pat::Ident(pat_ident) = pat {
            self.vars.insert(pat_ident.ident.clone());
        }
        syn::visit::visit_pat(self, pat);
    }
}