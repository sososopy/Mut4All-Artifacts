use std::error::Error;
use syn::{Expr, ExprIf};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{consts::IF_ELSE_B, info::env::ASTEnv},
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Block);
use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprIfStrategy;
impl FuzzerStrategyImpl<Expr> for ExprIfStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = ExprIf {
            attrs: vec![],
            if_token: gen_token!(if),
            cond: do_gen_name!(Expr)?.do_gen()?.into(),
            then_branch: do_gen_name!(Block)?.do_gen()?,
            else_branch: if IF_ELSE_B > glob_range(0. ..1.) {
                Some((gen_token!(else), do_gen_name!(Expr)?.do_gen()?.into()))
            } else {
                None
            },
        };
        Ok(Expr::If(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (50 * e.nested + 5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprIfStrategy, Expr, Expr::If(_));
