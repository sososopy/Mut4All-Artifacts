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

pub struct Inject_Generator_Closure_Iterating_274;

impl Mutator for Inject_Generator_Closure_Iterating_274 {
    fn name(&self) -> &str {
        "Inject_Generator_Closure_Iterating_274"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                if item_fn.sig.inputs.is_empty() {
                    continue;
                }
                let param_ident_opt = item_fn.sig.inputs.iter().find_map(|arg| {
                    if let syn::FnArg::Typed(pat_type) = arg {
                        if let syn::Pat::Ident(pat_ident) = &*pat_type.pat {
                            Some(pat_ident.ident.clone())
                        } else {
                            None
                        }
                    } else {
                        None
                    }
                });
                if let Some(param_ident) = param_ident_opt {
                    let closure: syn::ExprClosure = syn::parse_quote! {
                        || {
                            for i in #param_ident {
                                yield();
                            }
                        }
                    };
                    item_fn.block.stmts.push(syn::Stmt::Expr(syn::Expr::Closure(closure), None));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with at least one parameter. It selects one parameter and injects a generator-style closure at the end of the function body. The inserted closure iterates over the selected parameter and yields inside a loop. This transformation aims to test the compiler's handling of closures, generators, and const generic interactions within function bodies."
    }
}