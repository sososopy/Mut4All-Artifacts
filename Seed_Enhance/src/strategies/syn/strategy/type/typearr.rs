use std::error::Error;
use syn::{token::Bracket, Type, TypeArray};

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Type);
use_nodetype!(syn, Expr);

pub struct TypeArrayStrategy;
impl Default for TypeArrayStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Type> for TypeArrayStrategy {
    fn do_gen(&mut self) -> Result<Type, Box<dyn Error>> {
        let res = TypeArray {
            bracket_token: Bracket::default(),
            elem: do_gen_name!(Type)?.do_gen()?.into(),
            semi_token: gen_token!(;),
            len: do_gen_name!(Expr)?.do_gen()?
        };
        Ok(Type::Array(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.type_nest) as f64
    }
}

register_strategy!(TypeArrayStrategy, Type, Type::Array(..));
