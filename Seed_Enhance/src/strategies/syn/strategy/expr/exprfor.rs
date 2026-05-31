use std::error::Error;
use syn::{Expr, ExprForLoop};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{info::env::ASTEnv, other::lifetime::gen_label},
    use_nodetype,
};

use_nodetype!(syn, Block);
use_nodetype!(syn, Expr);
use_nodetype!(syn, Pat);

#[derive(Default)]
pub struct ExprForLoopStrategy;
impl FuzzerStrategyImpl<Expr> for ExprForLoopStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let e = ExprForLoop {
            attrs: vec![],
            label: gen_label(),
            for_token: gen_token!(for),
            pat: do_gen_name!(Pat)?.do_gen()?.into(),
            in_token: gen_token!(in),
            expr: do_gen_name!(Expr)?.do_gen()?.into(),
            body: do_gen_name!(Block)?.do_gen()?
        };
        Ok(Expr::ForLoop(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (50 * e.nested + 5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprForLoopStrategy, Expr, Expr::ForLoop(_));
