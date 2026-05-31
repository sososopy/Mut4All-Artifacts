use proc_macro2::{Span, TokenTree};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Lit, LitInt, Pat, PatType, Path as SynPath,
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

pub struct Alter_Numeric_Literal_Boundaries_21;

impl Mutator for Alter_Numeric_Literal_Boundaries_21 {
    fn name(&self) -> &str {
        "Alter_Numeric_Literal_Boundaries_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                for token in item_macro.mac.tokens.clone().into_iter() {
                    if let TokenTree::Literal(literal) = token {
                        if let Ok(lit_int) = literal.to_string().parse::<i32>() {
                            let new_literal = if literal.to_string().chars().all(char::is_numeric) {
                                LitInt::new(&format!("{}", i32::MAX), Span::call_site())
                            } else {
                                LitInt::new(&format!("{}{}", i32::MAX, &literal.to_string()[literal.to_string().find(|c: char| !c.is_numeric()).unwrap_or(literal.to_string().len())..]), Span::call_site())
                            };
                            item_macro.mac.tokens = parse_quote! { #new_literal };
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets numeric literals within macro definitions, replacing them with the maximum value for their type. This transformation is designed to test the compiler's handling of boundary values, which can reveal issues with overflow, range checks, and other edge case behaviors."
    }
}