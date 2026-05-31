use std::error::Error;
use proc_macro2::Span;
use syn::{Expr, ExprLit, Lit, LitInt};

use crate::{strategies::syn::strategy::FuzzerStrategyImpl, register_strategy, strategies::syn::strategy::info::env::ASTEnv, use_nodetype, util::glob_next};

use_nodetype!(syn, Expr);

pub struct ExprLitStrategy;
impl Default for ExprLitStrategy {
    fn default() -> Self{
        Self
    }
}
impl FuzzerStrategyImpl<Expr> for ExprLitStrategy {
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let num: i64 = glob_next();
        let res = Lit::Int(
            LitInt::new(num.to_string().as_str(), Span::mixed_site())
        );
        let res = ExprLit {
            attrs: Vec::new(),
            lit: res
        };
        let res = Expr::Lit(res);
        Ok(res)
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 0.5
    }
}

register_strategy!(ExprLitStrategy, Expr, Expr::Lit(_));