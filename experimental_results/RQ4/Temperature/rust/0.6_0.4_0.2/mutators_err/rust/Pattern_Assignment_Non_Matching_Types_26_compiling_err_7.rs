use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Pattern_Assignment_Non_Matching_Types_26;

impl Mutator for Pattern_Assignment_Non_Matching_Types_26 {
    fn name(&self) -> &str {
        "Pattern_Assignment_Non_Matching_Types_26"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let function_names: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| {
                if let Item::Fn(item_fn) = item {
                    Some(item_fn.sig.ident.clone())
                } else {
                    None
                }
            })
            .collect();

        for item in &mut file.items {
            if let Item::Stmt(Stmt::Local(local)) = item {
                if let Some(LocalInit { expr, .. }) = &mut local.init {
                    if let Expr::Tuple(expr_tuple) = expr.as_mut() {
                        if !function_names.is_empty() {
                            let random_function = function_names.choose(&mut rng).unwrap();
                            if !expr_tuple.elems.is_empty() {
                                expr_tuple.elems[0] = parse_quote!(#random_function);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets pattern assignments, specifically destructuring tuples, and replaces the first element with a randomly selected function name from the seed program. This introduces a type mismatch, testing the compiler's handling of pattern assignments and type inference when confronted with unexpected types in tuple destructuring."
    }
}