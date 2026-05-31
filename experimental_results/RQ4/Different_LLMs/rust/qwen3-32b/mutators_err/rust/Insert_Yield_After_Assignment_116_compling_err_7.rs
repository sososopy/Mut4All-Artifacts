use syn::parse_quote;
use crate::mutator::Mutator;
use syn::visit_mut::VisitMut;
use syn::ExprClosure;
use syn::Expr;
use syn::Stmt;

pub struct Insert_Yield_After_Assignment_116;

impl Mutator for Insert_Yield_After_Assignment_116 {
    fn name(&self) -> &str {
        "Insert_Yield_After_Assignment_116"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                self.process_block(&mut func.block);
            }
        }
        struct ClosureVisitor<'a> {
            me: &'a Insert_Yield_After_Assignment_116,
        }
        impl<'a> VisitMut for ClosureVisitor<'a> {
            fn visit_expr_closure_mut(&mut self, expr: &mut ExprClosure) {
                if let Expr::Block(expr_block) = &*expr.body {
                    self.me.process_block(&mut expr_block.block);
                }
                syn::visit_mut::visit_expr_closure_mut(self, expr);
            }
        }
        let mut visitor = ClosureVisitor { me: self };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts an additional `yield` statement after an existing `yield` in a generator, immediately following a variable assignment. This increases the complexity of the generator's state machine, potentially causing the compiler's drop-tracking logic to miscalculate drop ranges or state transitions, exposing bugs in the generator's state management."
    }
}

impl Insert_Yield_After_Assignment_116 {
    fn process_block(&self, block: &mut syn::Block) {
        let mut stmts = &mut block.stmts;
        let mut len = stmts.len();
        let mut i = 0;
        while i < len {
            let stmt = &stmts[i];
            if let Stmt::Expr(expr, _) = stmt {
                if let Expr::Yield(yield_expr) = expr {
                    if i + 1 < len {
                        let next_stmt = &stmts[i + 1];
                        if let Stmt::Expr(next_expr, _) = next_stmt {
                            if let Expr::Assign(assign_expr) = next_expr {
                                let new_yield = parse_quote! { yield; };
                                let new_stmt = Stmt::Expr(new_yield, None);
                                stmts.insert(i + 2, new_stmt);
                                len += 1;
                                i += 2;
                                continue;
                            }
                        }
                    }
                }
            }
            i += 1;
        }
    }
}