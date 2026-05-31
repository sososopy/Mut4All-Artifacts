use std::error::Error;
use syn::{Expr, ExprBlock};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, register_strategy,
    strategies::syn::strategy::{info::env::ASTEnv, other::lifetime::gen_label}, use_nodetype,
};

use_nodetype!(syn, Block);
use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprBlockStrategy;
impl FuzzerStrategyImpl<Expr> for ExprBlockStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = ExprBlock {
            attrs: vec![],
            label: gen_label(),
            block: do_gen_name!(Block)?.do_gen()?,
        };
        Ok(Expr::Block(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (50 * e.nested + 5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprBlockStrategy, Expr, Expr::Block(_));
