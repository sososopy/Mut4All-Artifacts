use std::error::Error;
use syn::{Expr, ExprReturn};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{consts::RET_EXPR_B, info::env::ASTEnv},
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprReturnStrategy;
impl FuzzerStrategyImpl<Expr> for ExprReturnStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = if RET_EXPR_B > glob_range(0. ..1.) {
            Some(Box::new(do_gen_name!(Expr)?.do_gen()?))
        } else {
            None
        };
        let e = ExprReturn {
            attrs: vec![],
            return_token: gen_token!(return),
            expr: e,
        };
        Ok(Expr::Return(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprReturnStrategy, Expr, Expr::Return(_));
