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
};

use crate::mutator::Mutator;

pub struct Swap_Function_Body_With_Recursive_Call_45;

impl Mutator for Swap_Function_Body_With_Recursive_Call_45 {
    fn name(&self) -> &str {
        "Swap_Function_Body_With_Recursive_Call_45"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let func_name = func.sig.ident.clone();
                    let args: Punctuated<Expr, Comma> = func.sig.inputs.iter().filter_map(|arg| {
                        if let FnArg::Typed(pat_type) = arg {
                            if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                return Some::<Expr>(parse_quote!(#pat_ident));
                            }
                        }
                        None
                    }).collect();

                    let recursive_call: Expr = parse_quote!(#func_name(#args));
                    let mut new_block = func.block.clone();
                    new_block.stmts.clear();
                    new_block.stmts.push(Stmt::Expr(recursive_call));
                    func.block = new_block;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the body of non-main functions with a recursive call to the function itself using its parameters. This transformation introduces infinite recursion, potentially leading to stack overflow, hangs, or triggering deep bugs in the compiler's handling of recursion and stack management."
    }
}