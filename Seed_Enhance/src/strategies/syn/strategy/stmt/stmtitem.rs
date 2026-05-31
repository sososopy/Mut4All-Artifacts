use std::error::Error;
use syn::Stmt;

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype,
};

use_nodetype!(syn, Stmt);
use_nodetype!(syn, Item);

pub struct StmtItemStrategy;
impl Default for StmtItemStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Stmt> for StmtItemStrategy {
    fn do_gen(&mut self) -> Result<Stmt, Box<dyn Error>> {
        let stra = do_gen_name!(Item)?;
        let res = Stmt::Item(stra.do_gen()?);
        Ok(res)
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (50. * e.stmt_nest as f64)
    }
}

register_strategy!(StmtItemStrategy, Stmt, Stmt::Item(..));
