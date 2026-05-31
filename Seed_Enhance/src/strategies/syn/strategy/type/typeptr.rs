use std::error::Error;
use syn::{Type, TypePtr};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{
        consts::{TYPE_PTR_CONST_B, TYPE_PTR_MUT_B},
        info::env::ASTEnv,
    },
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Type);

pub struct TypePtrStrategy;
impl Default for TypePtrStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Type> for TypePtrStrategy {
    fn do_gen(&mut self) -> Result<Type, Box<dyn Error>> {
        let res = TypePtr {
            star_token: gen_token!(*),
            const_token: if TYPE_PTR_CONST_B > glob_range(0. ..1.) {
                Some(gen_token!(const))
            } else {
                None
            },
            mutability: if TYPE_PTR_MUT_B > glob_range(0. ..1.) {
                Some(gen_token!(mut))
            } else {
                None
            },
            elem: do_gen_name!(Type)?.do_gen()?.into(),
        };
        Ok(Type::Ptr(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.type_nest) as f64
    }
}

register_strategy!(TypePtrStrategy, Type, Type::Ptr(..));
