use proc_macro2::Span;
use std::error::Error;
use syn::{Ident, Pat, PatIdent};

use crate::{
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{
        consts::{VAR_MUT_B, VAR_REF_B},
        info::env::{ASTEnv, current_env},
    },
    use_nodetype,
    util::{gen_alpha, glob_range},
};

use_nodetype!(syn, Pat);

pub struct PatIdentStrategy;
impl Default for PatIdentStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Pat> for PatIdentStrategy {
    fn do_gen(&mut self) -> Result<Pat, Box<dyn Error>> {
        let var = gen_alpha(5);
        {
            let e_ar = current_env();
            let mut e_lk = e_ar.write().map_err(|e| e.to_string())?;
            e_lk.insert_var(&var);
        }
        let var = Ident::new(&var, Span::call_site());
        let res = PatIdent {
            attrs: vec![],
            by_ref: if VAR_REF_B > glob_range(0. ..1.) {
                Some(gen_token!(ref))
            } else {
                None
            },
            mutability: if VAR_MUT_B > glob_range(0. ..1.) {
                Some(gen_token!(mut))
            } else {
                None
            },
            ident: var,
            subpat: None,
        };
        Ok(Pat::Ident(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.pat_nest) as f64
    }
}

register_strategy!(PatIdentStrategy, Pat, Pat::Ident(..));
