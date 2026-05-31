use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Modify_Closure_Capture_In_Const_Context_321;

impl Mutator for Modify_Closure_Capture_In_Const_Context_321 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_In_Const_Context_321"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_none() {
                    continue;
                }
                let mut closure_indices = Vec::new();
                for (index, stmt) in item_fn.block.stmts.iter().enumerate() {
                    if let Stmt::Expr(expr, _) = stmt {
                        if matches!(expr, Expr::Closure(_)) {
                            closure_indices.push(index);
                        }
                    }
                }
                if closure_indices.is_empty() {
                    continue;
                }
                let chosen_index = closure_indices
                    .choose(&mut thread_rng())
                    .copied()
                    .unwrap();
                let buffer_stmt: Stmt = parse_quote! {
                    let buffer: &[u8] = b"bar";
                };
                item_fn
                    .block
                    .stmts
                    .insert(chosen_index, buffer_stmt);
                if let Stmt::Expr(Expr::Closure(closure), _) =
                    &mut item_fn.block.stmts[chosen_index + 1]
                {
                    let sl_stmt: Stmt = parse_quote! {
                        let sl: &[u8] = buffer;
                    };
                    closure.body = Box::new(Expr::Block(ExprBlock {
                        attrs: Vec::new(),
                        label: None,
                        block: Block {
                            brace_token: closure.brace_token,
                            stmts: {
                                let mut stmts = closure
                                    .body
                                    .clone()
                                    .into_block()
                                    .stmts
                                    .into_iter()
                                    .collect::<Vec<_>>();
                                if let Some(index) = stmts.iter().position(|stmt| {
                                    if let Stmt::Local(local) = stmt {
                                        if let Some((_, expr)) = &local.init {
                                            if matches!(expr.as_ref(), Expr::Array(_)) {
                                                return true;
                                            }
                                        }
                                    }
                                    false
                                }) {
                                    stmts[index] = sl_stmt;
                                }
                                stmts
                            },
                        },
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets closures within const functions, modifying them to capture an external variable. It introduces a new variable outside the closure and rewrites the closure to use this variable, effectively changing the closure's capture behavior. This transformation stresses the compiler's handling of closure captures in const contexts, which are subject to stricter rules and limitations compared to non-const closures."
    }
}