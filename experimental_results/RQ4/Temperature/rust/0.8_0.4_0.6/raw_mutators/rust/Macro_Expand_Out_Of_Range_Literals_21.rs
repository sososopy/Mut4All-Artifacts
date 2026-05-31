use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Macro, Pat, PatType, Path as SynPath,
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

pub struct Macro_Expand_Out_Of_Range_Literals_21;

impl Mutator for Macro_Expand_Out_Of_Range_Literals_21 {
    fn name(&self) -> &str {
        "Macro_Expand_Out_Of_Range_Literals_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some(lit) = extract_literal_from_macro(mac) {
                    let new_lit = mutate_literal(lit);
                    replace_macro_literal(mac, new_lit);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets macros that output numeric literals. It modifies these literals to exceed their type's boundaries, such as changing an i32 maximum value to one beyond its limit. This tests the compiler's handling of edge-case literals, potentially uncovering bugs in literal parsing or macro expansion."
    }
}

fn extract_literal_from_macro(mac: &Macro) -> Option<i64> {
    // Simplified extraction logic for demonstration purposes
    if let Some(Expr::Lit(expr_lit)) = mac.tts.clone().into_iter().next() {
        if let syn::Lit::Int(lit_int) = expr_lit.lit {
            return lit_int.base10_parse::<i64>().ok();
        }
    }
    None
}

fn mutate_literal(literal: i64) -> i64 {
    literal.saturating_add(1)
}

fn replace_macro_literal(mac: &mut Macro, new_lit: i64) {
    mac.tts = quote! { #new_lit }.into();
}