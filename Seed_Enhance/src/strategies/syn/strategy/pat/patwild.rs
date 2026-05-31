use std::error::Error;
use syn::{Pat, PatWild};

use crate::{
    strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Pat);

pub struct PatWildStrategy;
impl Default for PatWildStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Pat> for PatWildStrategy {
    fn do_gen(&mut self) -> Result<Pat, Box<dyn Error>> {
        let res = PatWild {
            attrs: vec![],
            underscore_token: gen_token!(_)
        };
        Ok(Pat::Wild(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 1.
    }
}

register_strategy!(PatWildStrategy, Pat, Pat::Wild(..));
