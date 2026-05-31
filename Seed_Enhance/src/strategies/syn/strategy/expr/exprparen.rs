use std::error::Error;
use syn::{token::Paren, Expr, ExprParen};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprParenStrategy;
impl FuzzerStrategyImpl<Expr> for ExprParenStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = ExprParen {
            attrs: vec![],
            paren_token: Paren::default(),
            expr: do_gen_name!(Expr)?.do_gen()?.into()
        };
        Ok(Expr::Paren(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 0.5 / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprParenStrategy, Expr, Expr::Paren(_));
