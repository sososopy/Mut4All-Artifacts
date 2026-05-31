use std::error::Error;
use syn::{Expr, ExprBreak};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{consts::BREAK_EXPR_B, info::env::ASTEnv, other::lifetime::gen_lifetime},
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprBreakStrategy;
impl FuzzerStrategyImpl<Expr> for ExprBreakStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = if BREAK_EXPR_B > glob_range(0. ..1.) {
            Some(Box::new(do_gen_name!(Expr)?.do_gen()?))
        } else {
            None
        };
        let e = ExprBreak {
            attrs: vec![],
            break_token: gen_token!(break),
            label: gen_lifetime(),
            expr: e,
        };
        Ok(Expr::Break(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprBreakStrategy, Expr, Expr::Break(_));
