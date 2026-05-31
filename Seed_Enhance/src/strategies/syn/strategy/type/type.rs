use std::error::Error;
use syn::Type;

use crate::{
    do_gen_id,
    fuzz::rand_choose::WeightedRandDynamic,
    get_ids, register_strategy_no_kind,
    strategies::syn::strategy::{info::env::{current_env, ASTEnv}, FuzzerStrategyImpl},
    use_nodetype,
};

use_nodetype!(syn, Type);

pub struct TypeStrategy {
    rnd: WeightedRandDynamic<String, f64, ASTEnv>,
}
impl Default for TypeStrategy {
    fn default() -> Self {
        let all_items: Vec<_> = get_ids!(Type)
            .unwrap()
            .into_iter()
            .map(|x| (x.get_id().to_owned(), x.gen_weight()))
            .collect();
        Self {
            rnd: WeightedRandDynamic::new(Some(all_items), None),
        }
    }
}
impl FuzzerStrategyImpl<Type> for TypeStrategy {
    fn do_gen(&mut self) -> Result<Type, Box<dyn Error>> {
        let (id, _) = {
            let env_ar = current_env();
            let env_lk = env_ar.read().unwrap();
            let env = &*env_lk;
            self.rnd.rand(env)?
        };

        let stra = do_gen_id!(Type, id)?;

        {
            let env_ar = current_env();
            env_ar.write().unwrap().type_nest += 1;
        }
        let res = stra.do_gen()?;
        {
            let env_ar = current_env();
            env_ar.write().unwrap().type_nest -= 1;
        }

        Ok(res)
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 0.
    }
}

register_strategy_no_kind!(TypeStrategy, Type, 50);
