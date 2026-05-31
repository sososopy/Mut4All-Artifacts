use std::error::Error;
use syn::{Expr, Pat};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, register_strategy,
    strategies::syn::strategy::info::env::ASTEnv, use_nodetype,
};

use_nodetype!(syn, Pat);
use_nodetype!(syn, Expr);

pub struct PatRangeStrategy;
impl Default for PatRangeStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Pat> for PatRangeStrategy {
    fn do_gen(&mut self) -> Result<Pat, Box<dyn Error>> {
        let res = do_gen_name!(Expr, ExprRangeStrategy)?.do_gen()?;
        let res = match res {
            Expr::Range(r) => r,
            _ => unreachable!(),
        };
        Ok(Pat::Range(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 1.
    }
}

register_strategy!(PatRangeStrategy, Pat, Pat::Range(..));
