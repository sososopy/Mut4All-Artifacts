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

pub struct Modify_Literal_Values_In_Macros_21;

impl Mutator for Modify_Literal_Values_In_Macros_21 {
    fn name(&self) -> &str {
        "Modify_Literal_Values_In_Macros_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some(tokens) = mac.mac.tokens.clone().into_iter().next() {
                    if let TokenTree::Literal(lit) = tokens {
                        if let Ok(literal_value) = lit.to_string().parse::<i64>() {
                            let new_value = if literal_value.is_positive() {
                                1_000_000_000_000_000_000
                            } else {
                                -literal_value
                            };
                            mac.mac.tokens = quote! { #new_value };
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macros containing numeric literals, modifying them to either an extremely high value or an unexpected negative value. This transformation challenges the compiler's ability to handle extreme literal values and tests its diagnostic capabilities for out-of-range literals."
    }
}