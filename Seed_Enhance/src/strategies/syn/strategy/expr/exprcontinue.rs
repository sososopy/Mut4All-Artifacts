use std::error::Error;
use syn::{Expr, ExprContinue};

use crate::{
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{info::env::ASTEnv, other::lifetime::gen_lifetime},
    use_nodetype,
};

use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprContinueStrategy;
impl FuzzerStrategyImpl<Expr> for ExprContinueStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = ExprContinue {
            attrs: vec![],
            continue_token: gen_token!(continue),
            label: gen_lifetime()
        };
        Ok(Expr::Continue(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprContinueStrategy, Expr, Expr::Continue(_));
