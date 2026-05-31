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
                let mut rng = thread_rng();
                let param_index = rng.gen_range(0..func.sig.inputs.len());
                let param_name = match &func.sig.inputs[param_index] {
                    syn::FnArg::Typed(pat_type) => {
                        if let syn::Pat::Ident(pat_ident) = &*pat_type.pat {
                            pat_ident.ident.to_string()
                        } else {
                            continue;
                        }
                    }
                    _ => continue,
                };
                let closure_expr: syn::Expr = parse_quote! {
                    || {
                        for i in #param_name {
                            yield ();
                        }
                    }
                };
                func.block.stmts.push(syn::Stmt::Expr(closure_expr, None));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with at least one parameter and appends a generator closure to the function body. The closure iterates over a randomly selected parameter and yields inside the loop. This transformation stresses the compiler's handling of generator expressions, iterator patterns, and lifetime interactions with function parameters, potentially exposing bugs in closure capture, generator lowering, or MIR transformation."
    }
}