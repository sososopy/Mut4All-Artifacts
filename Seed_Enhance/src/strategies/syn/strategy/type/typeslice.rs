use std::error::Error;
use syn::{Type, TypeSlice, token::Bracket};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, register_strategy,
    strategies::syn::strategy::info::env::ASTEnv, use_nodetype,
};

use_nodetype!(syn, Type);

pub struct TypeSliceStrategy;
impl Default for TypeSliceStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Type> for TypeSliceStrategy {
    fn do_gen(&mut self) -> Result<Type, Box<dyn Error>> {
        let res = TypeSlice {
            bracket_token: Bracket::default(),
            elem: do_gen_name!(Type)?.do_gen()?.into(),
        };
        Ok(Type::Slice(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.type_nest) as f64
    }
}

register_strategy!(TypeSliceStrategy, Type, Type::Slice(..));
