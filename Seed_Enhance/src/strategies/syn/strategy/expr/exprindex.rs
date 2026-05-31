use std::error::Error;
use syn::{Expr, ExprIndex, token::Bracket};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, register_strategy,
    strategies::syn::strategy::info::env::ASTEnv, use_nodetype,
};

use_nodetype!(syn, Expr);

pub struct ExprIndexStrategy;
impl Default for ExprIndexStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Expr> for ExprIndexStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let res = ExprIndex {
            attrs: vec![],
            expr: do_gen_name!(Expr)?.do_gen()?.into(),
            bracket_token: Bracket::default(),
            index: do_gen_name!(Expr)?.do_gen()?.into(),
        };
        Ok(Expr::Index(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprIndexStrategy, Expr, Expr::Index(_));
