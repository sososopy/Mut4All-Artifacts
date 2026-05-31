use std::error::Error;
use syn::{Expr, ExprRange, RangeLimits};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{
        consts::{RANGE_END_B, RANGE_LIMITS_B, RANGE_START_B},
        info::env::ASTEnv,
    },
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Block);
use_nodetype!(syn, Expr);

#[derive(Default)]
pub struct ExprRangeStrategy;
impl FuzzerStrategyImpl<Expr> for ExprRangeStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let limits = if RANGE_LIMITS_B > glob_range(0. ..1.) {
            RangeLimits::HalfOpen(gen_token!(..))
        } else {
            RangeLimits::Closed(gen_token!(..=))
        };
        let start_end_b = glob_range(0. ..1.);
        // 0 RANGE_START_B RANGE_END_B 1
        let start = if RANGE_END_B > start_end_b {
            Some(do_gen_name!(Expr)?.do_gen()?.into())
        } else {
            None
        };
        let end = if matches!(limits, RangeLimits::Closed(..)) || RANGE_START_B < start_end_b {
            Some(do_gen_name!(Expr)?.do_gen()?.into())
        } else {
            None
        };
        let e = ExprRange {
            attrs: vec![],
            start,
            limits,
            end,
        };
        Ok(Expr::Range(e))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (50 * e.nested + 5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprRangeStrategy, Expr, Expr::Range(_));
