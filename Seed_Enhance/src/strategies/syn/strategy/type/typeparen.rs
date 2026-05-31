use std::error::Error;
use syn::{Type, TypeParen, token::Paren};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, register_strategy,
    strategies::syn::strategy::info::env::ASTEnv, use_nodetype,
};

use_nodetype!(syn, Type);

pub struct TypeParenStrategy;
impl Default for TypeParenStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Type> for TypeParenStrategy {
    fn do_gen(&mut self) -> Result<Type, Box<dyn Error>> {
        let res = TypeParen {
            paren_token: Paren::default(),
            elem: do_gen_name!(Type)?.do_gen()?.into(),
        };
        Ok(Type::Paren(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.type_nest) as f64
    }
}

register_strategy!(TypeParenStrategy, Type, Type::Paren(..));
