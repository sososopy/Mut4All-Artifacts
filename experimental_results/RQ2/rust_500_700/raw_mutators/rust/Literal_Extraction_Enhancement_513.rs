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

pub struct Literal_Extraction_Enhancement_513;

impl Mutator for Literal_Extraction_Enhancement_513 {
    fn name(&self) -> &str {
        "Literal_Extraction_Enhancement_513"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some(mac_rules) = &mac.mac.rules {
                    let mut new_rules = Vec::new();
                    for rule in mac_rules {
                        if let Some((_, body)) = &rule.body {
                            if let Expr::Lit(expr_lit) = &**body {
                                match &expr_lit.lit {
                                    syn::Lit::Str(_) => {
                                        new_rules.push(parse_quote!(() => { 42 }));
                                        new_rules.push(parse_quote!(() => { 3.14 }));
                                        new_rules.push(parse_quote!(() => { 'c' }));
                                        new_rules.push(parse_quote!(() => { true }));
                                        new_rules.push(parse_quote!(() => { 5 + 10 }));
                                    }
                                    _ => {}
                                }
                            }
                        }
                        new_rules.push(rule.clone());
                    }
                    mac.mac.rules = Some(new_rules);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}