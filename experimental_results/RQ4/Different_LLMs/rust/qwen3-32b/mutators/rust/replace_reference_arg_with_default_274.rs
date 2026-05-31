use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::{HashMap, HashSet}, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, ExprReference, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};
use syn::token::Semi;

use crate::mutator::Mutator;

pub struct Replace_Reference_Arg_With_Default_274;

impl Mutator for Replace_Reference_Arg_With_Default_274 {
    fn name(&self) -> &str {
        "Replace_Reference_Arg_With_Default_274"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut variables = HashMap::new();
                let block = &mut func.block;
                let mut var_collector = VarCollector { variables: &mut variables };
                var_collector.visit_block_mut(block);
                self.process_block(block, &variables);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces reference arguments in function calls with concrete values of the base type. It introduces type mismatches by substituting expected references with values, exploiting Rust's type inference and lifetime elision rules. This transformation challenges the compiler's ability to handle reference-to-value mismatches and may expose bugs in type coercion or higher-ranked trait bound resolution."
    }
}

struct VarCollector<'a> {
    variables: &'a mut HashMap<Ident, Type>,
}

impl<'a> VisitMut for VarCollector<'a> {
    fn visit_local_mut(&mut self, local: &mut Local) {
        if let Pat::Type(pat_type) = &local.pat {
            let ty = *pat_type.ty.clone();
            if let Pat::Ident(ident_pat) = &*pat_type.pat {
                self.variables.insert(ident_pat.ident.clone(), ty);
            }
        }
        syn::visit_mut::visit_local_mut(self, local);
    }
}

impl Replace_Reference_Arg_With_Default_274 {
    fn process_block(&self, block: &mut syn::Block, variables: &HashMap<Ident, Type>) {
        let mut insertions = Vec::new();
        let mut args_to_modify = Vec::new();
        
        for (stmt_index, stmt) in block.stmts.iter().enumerate() {
            if let Stmt::Expr(expr, _) = stmt {
                if let Expr::Call(call_expr) = expr {
                    for (arg_index, arg) in call_expr.args.iter().enumerate() {
                        if let Expr::Reference(ref_expr) = arg {
                            if let Expr::Path(path_expr) = ref_expr.expr.as_ref() {
                                if let Some(var_name) = path_expr.path.get_ident() {
                                    if let Some(var_type) = variables.get(&var_name) {
                                        let new_var_name = Ident::new(&format!("mutator_var_{}", var_name), Span::call_site());
                                        let default_expr = parse_quote!(<#var_type>::default());
                                        let new_var_decl = Local {
                                            attrs: vec![],
                                            let_token: Default::default(),
                                            pat: Pat::Ident(PatIdent {
                                                attrs: vec![],
                                                by_ref: None,
                                                mutability: None,
                                                ident: new_var_name.clone(),
                                                subpat: None,
                                            }),
                                            init: Some(LocalInit {
                                                eq_token: Default::default(),
                                                expr: Box::new(default_expr),
                                                diverge: None,
                                            }),
                                            semi_token: Semi {
                                                spans: [Span::call_site()],
                                            },
                                        };
                                        insertions.push((stmt_index, new_var_decl));
                                        args_to_modify.push((stmt_index, arg_index, new_var_name));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Insert new variables in reverse order to avoid index shifting
        for &(pos, ref new_var_decl) in insertions.iter().rev() {
            block.stmts.insert(pos, Stmt::Local(new_var_decl.clone()));
        }
        
        // Modify the arguments to use the new variables
        for (stmt_index, arg_index, new_var_name) in args_to_modify {
            if let Some(stmt) = block.stmts.get_mut(stmt_index) {
                if let Stmt::Expr(expr, _) = stmt {
                    if let Expr::Call(call_expr) = expr {
                        if let Some(arg) = call_expr.args.get_mut(arg_index) {
                            *arg = Expr::Path(ExprPath {
                                attrs: vec![],
                                qself: None,
                                path: new_var_name.into(),
                            });
                        }
                    }
                }
            }
        }
    }
}