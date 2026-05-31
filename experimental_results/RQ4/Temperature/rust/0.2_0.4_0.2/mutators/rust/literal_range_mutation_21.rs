use proc_macro2::{Span, TokenTree};
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

pub struct Literal_Range_Mutation_21;

impl Mutator for Literal_Range_Mutation_21 {
    fn name(&self) -> &str {
        "Literal_Range_Mutation_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                let mut tokens = item_macro.mac.tokens.clone().into_iter();
                if let Some(TokenTree::Literal(literal)) = tokens.next() {
                    if let Ok(lit_int) = literal.to_string().parse::<i64>() {
                        let new_value: i64 = 2_147_483_648; // Value exceeding i32 range
                        let new_lit = syn::Lit::Int(syn::LitInt::new(&new_value.to_string(), Span::call_site()));
                        item_macro.mac.tokens = quote! { #new_lit }.into();
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets numeric literals within macro definitions, specifically altering them to exceed typical i32 range. This transformation aims to test the compiler's handling of out-of-range literals and potential issues with range checks and error handling."
    }
}