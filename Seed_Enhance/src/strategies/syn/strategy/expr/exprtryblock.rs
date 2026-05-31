use std::error::Error;
use syn::{Expr, ExprTryBlock};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Block);
use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprTryBlockStrategy;
impl FuzzerStrategyImpl<Expr> for ExprTryBlockStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = ExprTryBlock {
            attrs: vec![],
            try_token: gen_token!(try),
            block: do_gen_name!(Block)?.do_gen()?,
        };
        Ok(Expr::TryBlock(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (50 * e.nested + 5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprTryBlockStrategy, Expr, Expr::TryBlock(_));
