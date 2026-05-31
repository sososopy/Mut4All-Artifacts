use std::error::Error;
use syn::{Expr, ExprConst};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Block);
use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprConstStrategy;
impl FuzzerStrategyImpl<Expr> for ExprConstStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = ExprConst {
            attrs: vec![],
            const_token: gen_token!(const),
            block: do_gen_name!(Block)?.do_gen()?
        };
        Ok(Expr::Const(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (50 * e.nested + 5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprConstStrategy, Expr, Expr::Const(_));
