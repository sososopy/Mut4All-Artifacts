use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{BoundLifetimes, Expr, File, Item};
struct Insert_Async_Closure_With_Capture_Mismatch_21;
impl Insert_Async_Closure_With_Capture_Mismatch_21 {
    fn mutate(&self, file_ast: &mut File) {
        let mut rng = thread_rng();
        let mut mutated = false;
        for item in &mut file_ast.items {
            if let Item::Fn(fn_item) = item {
                let block = &mut fn_item.block;
                let statements = &mut block.stmts;
                if statements.len() > 0 {
                    let idx = rng.gen_range(0..statements.len());
                    let target_stmt = &mut statements[idx];
                    if let syn::Stmt::Expr(expr) = target_stmt {
                        if let Expr::Assign(assign_expr) = expr {
                            let left = &assign_expr.left;
                            let right = &assign_expr.right;
                            if let Expr::Path(left_path) = left {
                                let var_name = left_path.path.segments.last().unwrap().ident.to_string();
                                let new_right = syn::parse_str::<Expr>(&format!("async || {{ {} }}", var_name)).unwrap();
                                *expr = syn::Stmt::Expr(syn::Expr::Assign(syn::ExprAssign {
                                    attrs: vec![],
                                    left: *left,
                                    right: Box::new(new_right),
                                    eq_token: syn::token::Eq::default(),
                                }));
                                mutated = true;
                            }
                        }
                    }
                }
            }
        }
        if !mutated {
            panic!("Mutator failed to apply mutation");
        }
    }
}