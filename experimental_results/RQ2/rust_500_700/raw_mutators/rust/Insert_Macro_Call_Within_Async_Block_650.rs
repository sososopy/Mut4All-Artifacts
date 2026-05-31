use proc_macro2::{Span, *};
use quote::*;
use syn::{
    Expr, File, Item, Stmt, parse_quote,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Insert_Macro_Call_Within_Async_Block_650;

impl Mutator for Insert_Macro_Call_Within_Async_Block_650 {
    fn name(&self) -> &str {
        "Insert_Macro_Call_Within_Async_Block_650"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Async(expr_async)) = stmt {
                        let macro_def: Item = parse_quote! {
                            macro_rules! my_macro {
                                () => {
                                    some_expression
                                };
                            }
                        };

                        let macro_call: Expr = parse_quote! {
                            my_macro!()
                        };

                        expr_async.block.stmts.insert(0, Stmt::Item(macro_def));
                        for stmt in &mut expr_async.block.stmts {
                            if let Stmt::Expr(Expr::Yield(expr_yield)) = stmt {
                                expr_yield.expr = Some(Box::new(macro_call.clone()));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}