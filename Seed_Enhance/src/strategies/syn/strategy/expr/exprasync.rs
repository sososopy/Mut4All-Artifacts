use std::error::Error;
use syn::{Expr, ExprAsync};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{
        consts::MOVE_R,
        info::env::ASTEnv,
    },
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Expr);
use_nodetype!(syn, Block);

#[derive(Default)]
pub struct ExprAsyncStrategy;
impl FuzzerStrategyImpl<Expr> for ExprAsyncStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let c = if MOVE_R > glob_range(0. ..1.) {
            Some(gen_token!(move))
        } else {
            None
        };
        let e = ExprAsync {
            attrs: vec![],
            async_token: gen_token!(async),
            capture: c,
            block: do_gen_name!(Block)?.do_gen()?,
        };
        Ok(Expr::Async(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (50 * e.nested + 5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprAsyncStrategy, Expr, Expr::Async(_));
