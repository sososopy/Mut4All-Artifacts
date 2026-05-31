use std::error::Error;
use syn::{Type, TypeInfer};

use crate::{
    strategies::syn::strategy::FuzzerStrategyImpl, gen_token, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Type);

pub struct TypeInferStrategy;
impl Default for TypeInferStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Type> for TypeInferStrategy {
    fn do_gen(&mut self) -> Result<Type, Box<dyn Error>> {
        let res = TypeInfer {
            underscore_token: gen_token!(_)
        };
        Ok(Type::Infer(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 1.
    }
}

register_strategy!(TypeInferStrategy, Type, Type::Infer(..));
