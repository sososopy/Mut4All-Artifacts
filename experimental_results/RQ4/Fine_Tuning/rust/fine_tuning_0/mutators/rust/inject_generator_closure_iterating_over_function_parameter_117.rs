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

pub struct Inject_Generator_Closure_Iterating_Over_Function_Parameter_117;

impl Mutator for Inject_Generator_Closure_Iterating_Over_Function_Parameter_117 {
    fn name(&self) -> &str {
        "Inject_Generator_Closure_Iterating_Over_Function_Parameter_117"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.inputs.is_empty() {
                    continue;
                }
                
                let param_ident = if let FnArg::Typed(pat_type) = &func.sig.inputs[0] {
                    if let Pat::Ident(pat_ident) = &*pat_type.pat {
                        pat_ident.ident.clone()
                    } else {
                        continue;
                    }
                } else {
                    continue;
                };

                let generator_closure: Expr = parse_quote! {
                    || {
                        for i in #param_ident {
                            yield();
                        }
                    }
                };

                func.block.stmts.push(Stmt::Expr(generator_closure, None));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with parameters and injects a generator-style closure that iterates over a selected parameter. By doing so, it tests the compiler's handling of generator expressions, iterator patterns, and lifetime interactions with generic parameters, potentially exposing bugs in closure capture, generator lowering, or MIR transformation."
    }
}