use std::error::Error;
use syn::Pat;

use crate::{
    do_gen_id,
    fuzz::rand_choose::WeightedRandDynamic,
    get_ids, register_strategy_no_kind,
    strategies::syn::strategy::{info::env::{current_env, ASTEnv}, FuzzerStrategyImpl},
    use_nodetype,
};

use_nodetype!(syn, Pat);

pub struct PatStrategy {
    rnd: WeightedRandDynamic<String, f64, ASTEnv>,
}
impl Default for PatStrategy {
    fn default() -> Self {
        let all_items: Vec<_> = get_ids!(Pat)
            .unwrap()
            .into_iter()
            .map(|x| (x.get_id().to_owned(), x.gen_weight()))
            .collect();
        Self {
            rnd: WeightedRandDynamic::new(Some(all_items), None),
        }
    }
}
impl FuzzerStrategyImpl<Pat> for PatStrategy {
    fn do_gen(&mut self) -> Result<Pat, Box<dyn Error>> {
        let (id, _) = {
            let env_ar = current_env();
            let env_lk = env_ar.read().unwrap();
            let env = &*env_lk;
            self.rnd.rand(env)?
        };

        {
            let env_ar = current_env();
            env_ar.write().unwrap().pat_nest += 1;
        }
        let stra = do_gen_id!(Pat, id)?;
        {
            let env_ar = current_env();
            env_ar.write().unwrap().pat_nest -= 1;
        }

        let res = stra.do_gen()?;

        Ok(res)
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 0.
    }
}

register_strategy_no_kind!(PatStrategy, Pat, 50);
