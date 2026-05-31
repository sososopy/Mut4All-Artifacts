use std::error::Error;
use syn::{Pat, PatType};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Pat);
use_nodetype!(syn, Type);

pub struct PatTypeStrategy;
impl Default for PatTypeStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Pat> for PatTypeStrategy {
    fn do_gen(&mut self) -> Result<Pat, Box<dyn Error>> {
        let res = PatType {
            attrs: vec![],
            pat: do_gen_name!(Pat, PatIdentStrategy)?.do_gen()?.into(),
            colon_token: gen_token!(:),
            ty: do_gen_name!(Type)?.do_gen()?.into()
        };
        Ok(Pat::Type(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.pat_nest) as f64
    }
}

register_strategy!(PatTypeStrategy, Pat, Pat::Type(..));
