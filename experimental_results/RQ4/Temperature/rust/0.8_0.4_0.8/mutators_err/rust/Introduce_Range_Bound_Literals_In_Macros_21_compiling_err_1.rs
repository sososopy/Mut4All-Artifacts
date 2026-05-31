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

pub struct Introduce_Range_Bound_Literals_In_Macros_21;

impl Mutator for Introduce_Range_Bound_Literals_In_Macros_21 {
    fn name(&self) -> &str {
        "Introduce_Range_Bound_Literals_In_Macros_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some((_, ref mut tokens)) = mac.mac.tokens.clone().into_iter().next() {
                    let mut new_tokens = tokens.clone();
                    if let Some(lit) = new_tokens.clone().into_iter().next() {
                        if let TokenTree::Literal(literal) = lit {
                            let value: i64 = literal.to_string().parse().unwrap_or(0);
                            let boundary_value = if value > 0 { i32::MAX } else { i32::MIN };
                            *tokens = parse_quote! { #boundary_value };
                        }
                    }
                }
            }
        }
        // Introduce a new macro if none exist
        if !file.items.iter().any(|item| matches!(item, Item::Macro(_))) {
            let new_macro: Item = parse_quote! {
                macro_rules! boundary_values {
                    () => { u8::MAX }
                }
            };
            file.items.push(new_macro);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macros that produce literal values, modifying them to use boundary values like i32::MAX or i32::MIN. If no such macros exist, it introduces a new macro that returns a boundary value, such as u8::MAX. This transformation aims to test the program's handling of edge case literals, potentially triggering logic related to 'literal out of range' checks."
    }
}