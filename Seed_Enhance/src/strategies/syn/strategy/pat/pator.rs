use std::error::Error;
use syn::{Pat, PatOr, punctuated::Punctuated};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{consts::{PAT_OR_LEAD_B, PAT_OR_R}, info::env::ASTEnv},
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Pat);

pub struct PatOrStrategy;
impl Default for PatOrStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Pat> for PatOrStrategy {
    fn do_gen(&mut self) -> Result<Pat, Box<dyn Error>> {
        let len = glob_range(PAT_OR_R);
        let mut inner = Punctuated::new();
        for _ in 0..len {
            inner.push(do_gen_name!(Pat)?.do_gen()?);
        }
        let res = PatOr {
            attrs: vec![],
            leading_vert: if PAT_OR_LEAD_B > glob_range(0. ..1.) {
                Some(gen_token!(|))
            } else {
                None
            },
            cases: inner,
        };
        Ok(Pat::Or(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.pat_nest) as f64
    }
}

register_strategy!(PatOrStrategy, Pat, Pat::Or(..));
