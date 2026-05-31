use std::error::Error;
use syn::{Pat, PatReference};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::{consts::TYPE_PTR_MUT_B, info::env::ASTEnv}, use_nodetype, util::glob_range
};

use_nodetype!(syn, Pat);

pub struct PatReferenceStrategy;
impl Default for PatReferenceStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Pat> for PatReferenceStrategy {
    fn do_gen(&mut self) -> Result<Pat, Box<dyn Error>> {
        let res = PatReference {
            attrs: vec![],
            and_token: gen_token!(&),
            mutability: if TYPE_PTR_MUT_B > glob_range(0. ..1.) {
                Some(gen_token!(mut))
            } else {
                None
            },
            pat: do_gen_name!(Pat)?.do_gen()?.into(),
        };
        Ok(Pat::Reference(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.pat_nest) as f64
    }
}

register_strategy!(PatReferenceStrategy, Pat, Pat::Reference(..));
