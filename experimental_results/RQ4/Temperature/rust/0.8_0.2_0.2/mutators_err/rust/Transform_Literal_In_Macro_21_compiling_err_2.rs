use proc_macro2::{Span, TokenStream, TokenTree};
use quote::ToTokens;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Transform_Literal_In_Macro_21;

impl Mutator for Transform_Literal_In_Macro_21 {
    fn name(&self) -> &str {
        "Transform_Literal_In_Macro_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(ref mut mac) = item {
                let mut rng = thread_rng();
                let new_value: Expr = match rng.gen_range(0..3) {
                    0 => parse_quote!(-1), // Change integer literal to -1
                    1 => parse_quote!(-1.0), // Change integer literal to -1.0 (float)
                    _ => parse_quote!("mutated"), // Change string literal
                };
                
                mac.mac.tokens = mac.mac.tokens.clone().into_iter().map(|token| {
                    if let TokenTree::Literal(lit) = &token {
                        if let Ok(literal) = syn::parse_str::<Expr>(&lit.to_string()) {
                            if matches!(literal, Expr::Lit(_)) {
                                let mut new_tokens = TokenStream::new();
                                new_value.to_tokens(&mut new_tokens);
                                return TokenTree::Group(proc_macro2::Group::new(proc_macro2::Delimiter::None, new_tokens));
                            }
                        }
                    }
                    token
                }).collect();
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro definitions containing literal values. It modifies these literals by changing their values or types, such as converting an integer to a negative float or altering a string. This transformation aims to test the compiler's handling of literal transformations in macro expansions, potentially exposing bugs in diagnostic or linting code paths."
    }
}