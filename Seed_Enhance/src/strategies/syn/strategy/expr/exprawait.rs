use std::error::Error;
use syn::{Expr, ExprAwait};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy,
    strategies::syn::strategy::info::env::ASTEnv, use_nodetype,
};

use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprAwaitStrategy;
impl FuzzerStrategyImpl<Expr> for ExprAwaitStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = ExprAwait {
            attrs: vec![],
            base: do_gen_name!(Expr)?.do_gen()?.into(),
            dot_token: gen_token!(.),
            await_token: gen_token!(await),
        };
        Ok(Expr::Await(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 0. / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprAwaitStrategy, Expr, Expr::Await(_));
