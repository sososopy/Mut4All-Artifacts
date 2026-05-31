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

pub struct Literal_Replacement_Mutator_199;

impl Mutator for Literal_Replacement_Mutator_199 {
    fn name(&self) -> &str {
        "Literal_Replacement_Mutator_199"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Lit(lit) = expr {
                            let new_lit = match lit.lit {
                                Lit::Int(ref int) => {
                                    let new_int = int.base10_parse::<i32>().unwrap_or(0) * 1000;
                                    if new_int > i32::MAX as i64 {
                                        Lit::Int(LitInt::new(&i32::MAX.to_string(), int.span()))
                                    } else {
                                        Lit::Int(LitInt::new(&new_int.to_string(), int.span()))
                                    }
                                }
                                Lit::Str(ref string) => {
                                    let new_string = format!("{}{}", string.value(), " modified");
                                    Lit::Str(LitStr::new(&new_string, string.span()))
                                }
                                Lit::Bool(ref bool) => {
                                    let new_bool = !bool.value;
                                    Lit::Bool(LitBool::new(new_bool, bool.span()))
                                }
                                _ => lit.lit.clone(),
                            };
                            *expr = parse_quote!(#new_lit);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The Literal Replacement Mutator replaces literal values in the code with new values that are likely to trigger errors, especially those related to lint checks and ICE errors. It applies to all literal values, including numeric, string, and boolean literals."
    }
}