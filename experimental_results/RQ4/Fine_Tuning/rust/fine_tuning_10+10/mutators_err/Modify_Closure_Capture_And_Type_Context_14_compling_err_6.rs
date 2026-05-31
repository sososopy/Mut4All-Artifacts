use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Modify_Closure_Capture_And_Type_Context_14;

impl Mutator for Modify_Closure_Capture_And_Type_Context_14 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_And_Type_Context_14"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut closure_found = false;
                let mut new_var_stmt_opt = None;
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr: init_expr, .. }) = &local.init {
                            if let Expr::Closure(closure) = &**init_expr {
                                closure_found = true;
                                let new_var_ident = Ident::new("y", Span::call_site());
                                let new_var_type: Type = parse_quote!(bool);
                                let new_var_expr: Expr = parse_quote!(true);
                                
                                let new_var_stmt: Stmt = parse_quote!(let #new_var_ident: #new_var_type = #new_var_expr;);
                                new_var_stmt_opt = Some(new_var_stmt);

                                if let Expr::Block(ref block) = *closure.body {
                                    let mut block = block.clone();
                                    let new_capture: Stmt = parse_quote!(if #new_var_ident { println!("Boolean captured!"); });
                                    block.block.stmts.push(new_capture);
                                    let new_closure_body = Expr::Block(block);
                                    *closure.body = new_closure_body;
                                }
                                break;
                            }
                        }
                    }
                }
                if let Some(new_var_stmt) = new_var_stmt_opt {
                    item_fn.block.stmts.insert(0, new_var_stmt);
                }
                if closure_found {
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies closures within function definitions, especially those used in complex contexts. It modifies the closure to capture an additional simple variable from the surrounding scope, adjusting the closure's type signature to reflect this new capture. This increases the likelihood of encountering issues related to closure captures and type inference."
    }
}