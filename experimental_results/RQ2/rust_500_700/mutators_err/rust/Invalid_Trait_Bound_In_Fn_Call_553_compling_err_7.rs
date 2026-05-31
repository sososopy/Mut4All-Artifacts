use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, ExprCall, File, Item, ItemFn, Stmt, parse_quote,
    punctuated::Punctuated,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Invalid_Trait_Bound_In_Fn_Call_553;

impl Mutator for Invalid_Trait_Bound_In_Fn_Call_553 {
    fn name(&self) -> &str {
        "Invalid_Trait_Bound_In_Fn_Call_553"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Call(expr_call)) = stmt {
                        if let Some(_) = expr_call.args.first() {
                            let invalid_trait_bound: Expr = parse_quote! { T: TraitName };
                            expr_call.args.push(invalid_trait_bound);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}