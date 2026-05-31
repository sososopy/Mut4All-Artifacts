use std::error::Error;
use syn::{Expr, ExprTry};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprTryStrategy;
impl FuzzerStrategyImpl<Expr> for ExprTryStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = ExprTry {
            attrs: vec![],
            expr: do_gen_name!(Expr)?.do_gen()?.into(),
            question_token: gen_token!(?),
        };
        Ok(Expr::Try(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 0.2 / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprTryStrategy, Expr, Expr::Try(_));
