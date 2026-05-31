use std::error::Error;
use syn::Expr;

use crate::{
    do_gen_id,
    fuzz::rand_choose::WeightedRandDynamic,
    get_ids, register_strategy_no_kind,
    strategies::syn::strategy::{info::env::{current_env, ASTEnv}, FuzzerStrategyImpl},
    use_nodetype,
};

use_nodetype!(syn, Expr);

pub struct ExprStrategy {
    rnd: WeightedRandDynamic<String, f64, ASTEnv>,
}
impl Default for ExprStrategy {
    fn default() -> Self {
        let all_items: Vec<_> = get_ids!(Expr)
            .unwrap()
            .into_iter()
            .map(|x| (x.get_id().to_owned(), x.gen_weight()))
            .collect();
        Self {
            rnd: WeightedRandDynamic::new(Some(all_items), None),
        }
    }
}
impl FuzzerStrategyImpl<Expr> for ExprStrategy {
    // fn do_fuzz(&self, v: &mut dyn Fold, node: Expr) -> Result<Expr, Box<dyn Error>> {
    //     match do_fuzz!(Expr, v, node) {
    //         Ok(DoFuzzRes::Success(nd)) => Ok(nd),
    //         Ok(DoFuzzRes::NoStreatgy(nd)) => Ok(fold_expr(v, nd)),
    //         Err(e) => {
    //             error!("Error: {:?}", e);
    //             panic!();
    //         }
    //     }
    // }
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let (id, _) = {
            let env_ar = current_env();
            let env_lk = env_ar.read().unwrap();
            let env = &*env_lk;
            self.rnd.rand(env)?
        };

        let stra = do_gen_id!(Expr, id)?;

        {
            let env_ar = current_env();
            env_ar.write().unwrap().expr_nest += 1;
        }
        let res = stra.do_gen()?;
        {
            let env_ar = current_env();
            env_ar.write().unwrap().expr_nest -= 1;
        }

        Ok(res)
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 0.
    }
}

register_strategy_no_kind!(ExprStrategy, Expr, 50);
