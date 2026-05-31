use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Modify_Closure_Capture_And_Type_Context_44;

impl Mutator for Modify_Closure_Capture_And_Type_Context_44 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_And_Type_Context_44"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                let mut closure_found = false;
                let mut new_var_decl = None;

                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Closure(closure) = &*init.expr {
                                closure_found = true;

                                let new_var: Local = parse_quote! {
                                    let y = true;
                                };

                                new_var_decl = Some(new_var);

                                let mut new_closure = closure.clone();
                                let new_expr: Expr = parse_quote! {
                                    if y { println!("Boolean captured!"); }
                                };

                                if let Expr::Block(block) = &mut *new_closure.body {
                                    block.block.stmts.push(Stmt::Expr(new_expr));
                                }

                                *init.expr = Expr::Closure(new_closure);
                            }
                        }
                    }
                }

                if closure_found {
                    if let Some(new_var) = new_var_decl {
                        func.block.stmts.insert(0, Stmt::Local(new_var));
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        ""
    }
}