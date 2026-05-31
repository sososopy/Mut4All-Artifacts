use std::error::Error;
use syn::Stmt;

use crate::{
    do_gen_name, strategies::syn::strategy::FuzzerStrategyImpl, register_strategy, rnd_token, strategies::syn::strategy::info::env::ASTEnv, use_nodetype
};

use_nodetype!(syn, Stmt);
use_nodetype!(syn, Expr);

pub struct StmtExprStrategy;
impl Default for StmtExprStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Stmt> for StmtExprStrategy {
    fn do_gen(&mut self) -> Result<Stmt, Box<dyn Error>> {
        let semi = rnd_token!(;);
        let stra = do_gen_name!(Expr)?;
        let res = Stmt::Expr(stra.do_gen()?, semi);
        Ok(res)
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 1.
    }
}

register_strategy!(StmtExprStrategy, Stmt, Stmt::Expr(..));
