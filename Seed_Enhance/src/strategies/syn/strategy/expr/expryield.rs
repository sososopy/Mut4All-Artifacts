use std::error::Error;
use syn::{Expr, ExprYield};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype, util::glob_range
};

use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprYieldStrategy;
impl FuzzerStrategyImpl<Expr> for ExprYieldStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = match glob_range(0..1) {
            0 => Some(
                Box::new(
                    do_gen_name!(Expr)?.do_gen()?
                )
            ),
            1 => None,
            _ => unreachable!(),
        };
        let e = ExprYield {
            attrs: vec![],
            yield_token: gen_token!(yield),
            expr: e,
        };
        Ok(Expr::Yield(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprYieldStrategy, Expr, Expr::Yield(_));
