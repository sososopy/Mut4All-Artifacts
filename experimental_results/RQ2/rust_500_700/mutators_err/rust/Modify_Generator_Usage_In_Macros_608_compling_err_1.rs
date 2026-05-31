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

pub struct Modify_Generator_Usage_In_Macros_608;

impl Mutator for Modify_Generator_Usage_In_Macros_608 {
    fn name(&self) -> &str {
        "Modify_Generator_Usage_In_Macros_608"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Macro(expr_macro)) = stmt {
                        if let Some((_, expr)) = expr_macro.mac.tokens.clone().into_iter().next() {
                            if let Expr::Closure(closure) = expr {
                                let generator_var: Ident = Ident::new("gen", Span::call_site());
                                let generator_decl: Stmt = parse_quote! {
                                    let #generator_var = #closure;
                                };
                                new_stmts.push(generator_decl);
                                let new_macro: Stmt = parse_quote! {
                                    #expr_macro.mac.path!(#generator_var);
                                };
                                new_stmts.push(new_macro);
                                continue;
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                func.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}