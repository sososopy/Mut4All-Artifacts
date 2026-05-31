use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
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

pub struct Replace_Recursive_Call_With_Loop_156;

impl Mutator for Replace_Recursive_Call_With_Loop_156 {
    fn name(&self) -> &str {
        "Replace_Recursive_Call_With_Loop_156"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut recursive_calls = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call) = expr {
                            if let Expr::Path(path) = &*call.func {
                                if path.path.segments.last().unwrap().ident == func.sig.ident {
                                    recursive_calls.push(call);
                                }
                            }
                        }
                    }
                }

                if !recursive_calls.is_empty() {
                    let mut new_stmts = Vec::new();
                    for stmt in &func.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Call(call) = expr {
                                if let Expr::Path(path) = &*call.func {
                                    if path.path.segments.last().unwrap().ident == func.sig.ident {
                                        let args = call.args.clone();
                                        let mut new_stmt = Stmt::Expr(
                                            parse_quote! {
                                                let mut current_state = #args;
                                            },
                                            None,
                                        );
                                        new_stmts.push(new_stmt);

                                        let mut loop_cond = parse_quote! {
                                            true
                                        };
                                        let mut loop_body = Block {
                                            brace_token: token::Brace {
                                                span: Span::call_site(),
                                                token: token::Brace {
                                                    span: Span::call_site(),
                                                },
                                            },
                                            stmts: {
                                                let mut stmts = Vec::new();
                                                for arg in args {
                                                    stmts.push(Stmt::Expr(
                                                        parse_quote! {
                                                            current_state = #arg;
                                                        },
                                                        None,
                                                    ));
                                                }
                                                stmts
                                            },
                                        };

                                        new_stmt = Stmt::Expr(
                                            parse_quote! {
                                                loop {
                                                    #loop_body
                                                }
                                            },
                                            None,
                                        );
                                        new_stmts.push(new_stmt);
                                    } else {
                                        new_stmts.push(stmt.clone());
                                    }
                                } else {
                                    new_stmts.push(stmt.clone());
                                }
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces recursive function calls with a loop-based version, aiming to expose bugs related to stack overflow or incorrect recursive function calls."
    }
}