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
                    if let syn::Stmt::Expr(expr, _) = target_stmt {
                        if let Expr::Assign(assign_expr) = expr {
                            let left = &assign_expr.left;
                            let right = &assign_expr.right;
                            if let Expr::Path(left_path) = &**left {
                                let var_name = left_path.path.segments.last().unwrap().ident.to_string();
                                let new_right = syn::parse_str::<Expr>(&format!("async || {{ {} }}", var_name)).unwrap();
                                *expr = syn::Expr::Assign(syn::ExprAssign {
                                    attrs: vec![],
                                    left: left.clone(),
                                    right: Box::new(new_right),
                                    eq_token: syn::token::Eq::default(),
                                });
                                mutated = true;
                            }
                        }
                    }
                }
            }
        }
        if !mutated {
            let mut rng = thread_rng();
            for item in &mut file_ast.items {
                if let Item::Fn(fn_item) = item {
                    let block = &mut fn_item.block;
                    let statements = &mut block.stmts;
                    if statements.len() > 0 {
                        let idx = rng.gen_range(0..statements.len());
                        let target_stmt = &mut statements[idx];
                        if let syn::Stmt::Expr(expr, _) = target_stmt {
                            let var_name = "x";
                            let new_right = syn::parse_str::<Expr>(&format!("async || {{ {} }}", var_name)).unwrap();
                            *expr = syn::Expr::Assign(syn::ExprAssign {
                                attrs: vec![],
                                left: Box::new(syn::Expr::Path(syn::ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: syn::Path {
                                        segments: syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                            ident: syn::Ident::new(var_name, Span::call_site()),
                                            arguments: syn::PathArguments::None,
                                        }]),
                                    },
                                })),
                                right: Box::new(new_right),
                                eq_token: syn::token::Eq::default(),
                            });
                            mutated = true;
                            break;
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