use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
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

use crate::mutator::Mutator;

pub struct Modify_Tuple_Pattern_Assignment_240;

impl Mutator for Modify_Tuple_Pattern_Assignment_240 {
    fn name(&self) -> &str {
        "Modify_Tuple_Pattern_Assignment_240"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut has_tuple_assignment = false;
                        for stmt in &func.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Pat::Tuple(_) = &local.pat {
                                    has_tuple_assignment = true;
                                    break;
                                }
                            }
                        }
                        if !has_tuple_assignment {
                            continue;
                        }
                        let mut new_stmts = Vec::new();
                        for stmt in &func.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Pat::Tuple(pat_tuple) = &local.pat {
                                    let mut new_pat_elems = pat_tuple.elems.clone();
                                    let mut new_init_elems = Punctuated::new();
                                    if let Some(LocalInit { expr, .. }) = &local.init {
                                        if let Expr::Tuple(expr_tuple) = &**expr {
                                            new_init_elems = expr_tuple.elems.clone();
                                        }
                                    }
                                    if new_pat_elems.len() != new_init_elems.len() {
                                        new_stmts.push(stmt.clone());
                                        continue;
                                    }
                                    if new_pat_elems.len() == 0 {
                                        new_stmts.push(stmt.clone());
                                        continue;
                                    }
                                    let mut rng = thread_rng();
                                    let mut mismatch_index = rng.gen_range(0..new_pat_elems.len());
                                    let mut used_names = HashSet::new();
                                    for elem in &new_pat_elems {
                                        if let Pat::Ident(pat_ident) = elem {
                                            used_names.insert(pat_ident.ident.to_string());
                                        }
                                    }
                                    for elem in &new_init_elems {
                                        if let Expr::Path(expr_path) = elem {
                                            if expr_path.qself.is_none()
                                                && expr_path.path.segments.len() == 1
                                            {
                                                used_names.insert(
                                                    expr_path.path.segments[0]
                                                        .ident
                                                        .to_string(),
                                                );
                                            }
                                        }
                                    }
                                    let mut new_var_name = String::new();
                                    for i in 0.. {
                                        let candidate = format!("__mutated_var{}", i);
                                        if !used_names.contains(&candidate) {
                                            new_var_name = candidate;
                                            break;
                                        }
                                    }
                                    let mut new_var_type = parse_quote!(i32);
                                    if let Some(LocalInit { expr, .. }) = &local.init {
                                        if let Expr::Tuple(expr_tuple) = &**expr {
                                            if let Some(first_elem) = expr_tuple.elems.first() {
                                                if let Expr::Path(expr_path) = first_elem {
                                                    if expr_path.qself.is_none()
                                                        && expr_path.path.segments.len() == 1
                                                    {
                                                        let first_ident =
                                                            &expr_path.path.segments[0].ident;
                                                        if first_ident == "S" {
                                                            new_var_type = parse_quote!(S);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    let new_var_decl: Stmt = parse_quote! {
                                        let #new_var_name: #new_var_type = 0;
                                    };
                                    new_stmts.push(new_var_decl);
                                    if let Pat::Ident(pat_ident) = &new_pat_elems[mismatch_index] {
                                        let new_ident = Ident::new(&new_var_name, pat_ident.span());
                                        new_pat_elems[mismatch_index] =
                                            Pat::Ident(PatIdent { ident: new_ident, ..*pat_ident });
                                    }
                                    if let Expr::Path(expr_path) = &new_init_elems[mismatch_index] {
                                        let new_ident =
                                            Ident::new(&new_var_name, expr_path.span());
                                        new_init_elems[mismatch_index] = Expr::Path(ExprPath {
                                            path: syn::Path::from(new_ident),
                                            ..expr_path.clone()
                                        });
                                    }
                                    let new_local = Local {
                                        pat: Pat::Tuple(PatTuple {
                                            elems: new_pat_elems,
                                            ..pat_tuple.clone()
                                        }),
                                        init: Some(LocalInit {
                                            eq_token: token::Eq {
                                                spans: [Span::call_site()],
                                            },
                                            expr: Box::new(Expr::Tuple(ExprTuple {
                                                elems: new_init_elems,
                                                ..ExprTuple {
                                                    attrs: Vec::new(),
                                                    paren_token: Paren {
                                                        span: proc_macro2::Span::call_site()
                                                            .into(),
                                                    },
                                                    elems: Punctuated::new(),
                                                }
                                            })),
                                            diverge: None,
                                        }),
                                        ..local.clone()
                                    };
                                    new_stmts.push(Stmt::Local(new_local));
                                } else {
                                    new_stmts.push(stmt.clone());
                                }
                            } else {
                                new_stmts.push(stmt.clone());
                            }
                        }
                        func.block.stmts = new_stmts;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets tuple pattern assignments within impl block functions. It introduces a type mismatch by altering one element in both the pattern and the right-hand side expression. The mutator ensures that the modified element uses an existing or newly declared variable, creating a deliberate type inconsistency to stress-test the compiler's pattern matching and type inference mechanisms."
    }
}