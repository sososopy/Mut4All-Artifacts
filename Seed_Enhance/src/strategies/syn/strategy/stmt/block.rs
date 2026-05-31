use std::error::Error;
use syn::{Block, Stmt, token::Brace};

use crate::{
    do_gen_name, register_strategy_no_kind,
    strategies::syn::strategy::{consts::BLOCK_STMT_R, info::env::{current_env, nest_env, unnest_env, ASTEnv}, FuzzerStrategyImpl},
    use_nodetype,
    util::glob_range,
};

use_nodetype!(syn, Block);
use_nodetype!(syn, Expr);
use_nodetype!(syn, Stmt);

#[derive(Default)]
pub struct BlockStrategy {}
impl FuzzerStrategyImpl<Block> for BlockStrategy {
    fn do_gen(&mut self) -> Result<Block, Box<dyn Error>> {
        let nest_now = {
            let env_ar = current_env();
            let env_lk = env_ar.read().unwrap();
            let env = &*env_lk;
            env.nested
        };

        let stmt_ub = BLOCK_STMT_R.end - nest_now as i64;
        let stmt_sb = BLOCK_STMT_R.start;

        let stmt_cnt = if stmt_ub <= stmt_sb {
            1
        } else {
            glob_range(stmt_sb..stmt_ub)
        };

        nest_env()?;

        let mut stmts: Vec<Stmt> = vec![];
        for _ in 0..stmt_cnt {
            stmts.push(do_gen_name!(Stmt)?.do_gen()?);
        }

        unnest_env()?;

        Ok(Block {
            brace_token: Brace::default(),
            stmts,
        })
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 0.
    }
}

register_strategy_no_kind!(BlockStrategy, Block, 50);
