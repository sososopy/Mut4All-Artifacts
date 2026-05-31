use quote::quote;
use syn::{File, Item, ItemFn, Stmt, Expr, Arm, Pat, Block};
use syn::visit_mut::VisitMut;
use syn::spanned::Spanned;

struct Replace_Match_With_If_Else_Chain_225;

impl Mutator for Replace_Match_With_If_Else_Chain_225 {
    fn name(&self) -> &str {
        "Replace_Match_With_If_Else_Chain_225"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Match(expr_match) = expr {
                            let mut if_else_chain = Vec::new();
                            for arm in &expr_match.arms {
                                let pattern = &arm.pat;
                                let body = &arm.body;
                                let if_stmt = quote! {
                                    if #pattern == #expr_match.expr {
                                        #body
                                    }
                                };
                                if_else_chain.push(if_stmt);
                            }
                            let if_else_chain = if_else_chain.into_iter().reduce(|acc, elem| {
                                quote! {
                                    #acc
                                    #elem
                                }
                            });
                            if let Some(if_else_chain) = if_else_chain {
                                *stmt = Stmt::Expr(syn::Expr::Verbatim(if_else_chain), expr.__span());
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces match statements with if-else chains. This transformation tests the compiler's handling of control flow and conditional statements, potentially revealing issues in optimization or validation phases."
    }
}

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut File);
    fn chain_of_thought(&self) -> &str;
}