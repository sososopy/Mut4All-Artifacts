use std::error::Error;
use syn::{Type, TypeReference};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{
        consts::TYPE_PTR_MUT_B,
        info::env::ASTEnv, other::lifetime::gen_lifetime,
    },
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Type);

pub struct TypeReferenceStrategy;
impl Default for TypeReferenceStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Type> for TypeReferenceStrategy {
    fn do_gen(&mut self) -> Result<Type, Box<dyn Error>> {
        let res = TypeReference {
            and_token: gen_token!(&),
            lifetime: gen_lifetime(),
            mutability: if TYPE_PTR_MUT_B > glob_range(0. ..1.) {
                Some(gen_token!(mut))
            } else {
                None
            },
            elem: do_gen_name!(Type)?.do_gen()?.into(),
        };
        Ok(Type::Reference(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.type_nest) as f64
    }
}

register_strategy!(TypeReferenceStrategy, Type, Type::Reference(..));
