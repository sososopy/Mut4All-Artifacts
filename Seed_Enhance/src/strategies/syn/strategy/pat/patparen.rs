use std::error::Error;
use syn::{token::Paren, Pat, PatParen};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Pat);

pub struct PatParenStrategy;
impl Default for PatParenStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Pat> for PatParenStrategy {
    fn do_gen(&mut self) -> Result<Pat, Box<dyn Error>> {
        let res = PatParen {
            attrs: vec![],
            paren_token: Paren::default(),
            pat: do_gen_name!(Pat)?.do_gen()?.into()
        };
        Ok(Pat::Paren(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.pat_nest) as f64
    }
}

register_strategy!(PatParenStrategy, Pat, Pat::Paren(..));
