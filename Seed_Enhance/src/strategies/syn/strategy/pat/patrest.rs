use std::error::Error;
use syn::{Pat, PatRest};

use crate::{
    strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv,
    use_nodetype,
};

use_nodetype!(syn, Pat);

pub struct PatRestStrategy;
impl Default for PatRestStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Pat> for PatRestStrategy {
    fn do_gen(&mut self) -> Result<Pat, Box<dyn Error>> {
        let res = PatRest {
            attrs: vec![],
            dot2_token: gen_token!(..),
        };
        Ok(Pat::Rest(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 1.
    }
}

register_strategy!(PatRestStrategy, Pat, Pat::Rest(..));
