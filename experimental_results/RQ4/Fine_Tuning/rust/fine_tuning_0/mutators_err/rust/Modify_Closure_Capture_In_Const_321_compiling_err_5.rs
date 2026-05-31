use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
    parse::{Parse, ParseStream},
    ExprClosure, ExprBlock,
};

use crate::mutator::Mutator;

pub struct Modify_Closure_Capture_In_Const_321;

impl Mutator for Modify_Closure_Capture_In_Const_321 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_In_Const_321"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    let mut new_stmts = Vec::new();
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Closure(closure)) = stmt {
                            let capture_var: Stmt = parse_quote! { let buffer: &[u8] = b"bar"; };
                            let mut new_closure = closure.clone();
                            if let Expr::Block(expr_block) = &mut *new_closure.body {
                                expr_block.block.stmts.insert(0, parse_quote! {
                                    let sl: &[u8] = buffer;
                                });
                            }
                            new_stmts.push(capture_var);
                            new_stmts.push(Stmt::Expr(Expr::Closure(new_closure)));
                        } else {
                            new_stmts.push(stmt.clone());
                        }
                    }
                    item_fn.block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies closures within const functions and modifies their capture mechanism by introducing an external variable. This change ensures the closure captures the variable, testing the compiler's handling of closure captures in const contexts."
    }
}