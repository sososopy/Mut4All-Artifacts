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

pub struct Iterate_Mutable_Ref_Conditional_Mod_611;

impl Mutator for Iterate_Mutable_Ref_Conditional_Mod_611 {
    fn name(&self) -> &str {
        "Iterate_Mutable_Ref_Conditional_Mod_611"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Loop(expr_loop), _) = stmt {
                        let mut modified = false;
                        let mut new_loop_stmts = Vec::new();
                        for loop_stmt in &expr_loop.body.stmts {
                            if let Stmt::Expr(Expr::Call(ExprCall { func, args, .. }), _) = loop_stmt {
                                if let Expr::Path(ExprPath { path, .. }) = &**func {
                                    if path.segments.last().unwrap().ident == "do_something" {
                                        if let Some(Expr::Reference(expr_ref)) = args.first() {
                                            if expr_ref.mutability.is_some() {
                                                let temp_ident = Ident::new("temp", Span::call_site());
                                                new_loop_stmts.push(parse_quote! {
                                                    let #temp_ident = #expr_ref;
                                                });
                                                new_loop_stmts.push(parse_quote! {
                                                    do_something(#temp_ident);
                                                });
                                                modified = true;
                                                continue;
                                            }
                                        }
                                    }
                                }
                            }
                            new_loop_stmts.push(loop_stmt.clone());
                        }
                        if modified {
                            new_stmts.push(Stmt::Expr(Expr::Loop(ExprLoop {
                                body: Block {
                                    stmts: new_loop_stmts,
                                    ..expr_loop.body.clone()
                                },
                                ..expr_loop.clone()
                            }), None));
                            continue;
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                func.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets loops iterating over mutable references and introduces a temporary variable to hold the mutable reference. The operation is then applied to this temporary variable, testing the borrow checker's behavior with temporary reassignments in iterative structures."
    }
}