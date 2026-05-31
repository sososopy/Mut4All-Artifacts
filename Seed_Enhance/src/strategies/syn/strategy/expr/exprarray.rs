use std::error::Error;
use syn::{Expr, ExprArray, punctuated::Punctuated, token::Bracket};

use crate::{
    do_gen_name, register_strategy,
    strategies::syn::strategy::{FuzzerStrategyImpl, consts::ARR_R, info::env::ASTEnv},
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Expr);
use_nodetype!(syn, Block);

#[derive(Default)]
pub struct ExprAsyncStrategy;
impl FuzzerStrategyImpl<Expr> for ExprAsyncStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let elems_cnt = glob_range(ARR_R);
        let mut elems = Punctuated::new();
        for _ in 0..elems_cnt {
            elems.push(do_gen_name!(Expr)?.do_gen()?);
        }
        let res = ExprArray {
            attrs: vec![],
            bracket_token: Bracket::default(),
            elems,
        };
        Ok(Expr::Array(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (50 * e.nested + 5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprAsyncStrategy, Expr, Expr::Async(_));
