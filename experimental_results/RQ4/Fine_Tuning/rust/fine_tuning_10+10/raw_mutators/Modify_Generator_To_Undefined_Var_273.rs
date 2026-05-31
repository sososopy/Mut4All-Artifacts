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

pub struct Modify_Generator_To_Undefined_Var_273;

impl Mutator for Modify_Generator_To_Undefined_Var_273 {
    fn name(&self) -> &str {
        "Modify_Generator_To_Undefined_Var_273"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if is_generator_function(item_fn) {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some((_, expr)) = &mut local.init {
                                *expr = parse_quote!(undefined_var);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets generator functions, identified by the use of the `yield` keyword. It modifies a variable assignment within the generator function, replacing the right-hand side of the assignment with a reference to an undefined variable, `undefined_var`. This change is designed to trigger a compilation error due to the unresolved reference, testing the compiler's ability to handle such errors in generator contexts."
    }
}

fn is_generator_function(item_fn: &ItemFn) -> bool {
    item_fn.block.stmts.iter().any(|stmt| {
        if let Stmt::Expr(Expr::Yield(_)) = stmt {
            true
        } else {
            false
        }
    })
}