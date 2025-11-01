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

pub struct Invalid_Method_Call_On_Non_Adt_21;

impl Mutator for Invalid_Method_Call_On_Non_Adt_21 {
    fn name(&self) -> &str {
        "Invalid_Method_Call_On_Non_Adt_21"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(ref mut local) = stmt {
                        if let Some((_, ref mut expr)) = local.init {
                            if let Expr::Array(_) | Expr::Tuple(_) = **expr {
                                let invalid_call: Expr = parse_quote! {
                                    .invalid_method()
                                };
                                let new_expr = Expr::Verbatim(quote! { #expr #invalid_call });
                                *expr = Box::new(new_expr);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets non-ADT constructs like arrays and tuples. It introduces an invalid method call to these constructs to test the compiler's error handling. By appending a non-existent method call to arrays or tuples, we aim to generate compile-time errors, ensuring robustness in const-eval and error reporting."
    }
}