use std::error::Error;
use syn::{
    Expr, ExprUnary, UnOp
};

use crate::{do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype, util::glob_range};

use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprUnaryStrategy;
impl FuzzerStrategyImpl<Expr> for ExprUnaryStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let r = glob_range(0..3);
        let op = match r {
            0 => UnOp::Deref(gen_token!(*)),
            1 => UnOp::Not(gen_token!(!)),
            2 => UnOp::Neg(gen_token!(-)),
            _ => unreachable!()
        };
        let e = ExprUnary {
            attrs: vec![],
            op,
            expr: do_gen_name!(Expr)?.do_gen()?.into()
        };
        Ok(Expr::Unary(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 0.8 / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprUnaryStrategy, Expr, Expr::Unary(_));
