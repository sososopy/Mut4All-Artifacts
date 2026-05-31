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

pub struct Add_Indirect_Recursive_Call_154;

impl Mutator for Add_Indirect_Recursive_Call_154 {
    fn name(&self) -> &str {
        "Add_Indirect_Recursive_Call_154"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref return_type) = func.sig.output {
                    if let syn::Type::ImplTrait(_) = **return_type {
                        if let Some(recursive_call) = find_recursive_call(&func.sig.ident, &func.block) {
                            let new_func_name = Ident::new(&format!("indirect_{}", func.sig.ident), Span::call_site());
                            let mut new_func = func.clone();
                            new_func.sig.ident = new_func_name.clone();
                            new_func.block = Box::new(parse_quote!({
                                #recursive_call
                            }));

                            for stmt in &mut func.block.stmts {
                                if let syn::Stmt::Expr(syn::Expr::Call(call), _) = stmt {
                                    if let syn::Expr::Path(ref path) = *call.func {
                                        if path.path.is_ident(&func.sig.ident) {
                                            call.func = Box::new(parse_quote!(#new_func_name));
                                        }
                                    }
                                }
                            }
                            new_items.push(syn::Item::Fn(new_func));
                        }
                    }
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies functions with `impl Trait` return types that call themselves recursively. It introduces an additional function as an intermediary, redirecting recursive calls through this new function. This transformation adds complexity to the call graph, potentially exposing issues in recursive trait resolution and function inlining."
    }
}

fn find_recursive_call(func_name: &Ident, block: &syn::Block) -> Option<syn::ExprCall> {
    for stmt in &block.stmts {
        if let syn::Stmt::Expr(syn::Expr::Call(call), _) = stmt {
            if let syn::Expr::Path(ref path) = *call.func {
                if path.path.is_ident(func_name) {
                    return Some(call.clone());
                }
            }
        }
    }
    None
}