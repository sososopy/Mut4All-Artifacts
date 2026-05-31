use std::error::Error;
use syn::{Expr, ExprInfer};

use crate::{
    strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv,
    use_nodetype,
};

use_nodetype!(syn, Expr);

pub struct ExprInferStrategy;
impl Default for ExprInferStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Expr> for ExprInferStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let res = ExprInfer {
            attrs: vec![],
            underscore_token: gen_token!(_),
        };
        Ok(Expr::Infer(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 0.25
    }
}

register_strategy!(ExprInferStrategy, Expr, Expr::Infer(_));
