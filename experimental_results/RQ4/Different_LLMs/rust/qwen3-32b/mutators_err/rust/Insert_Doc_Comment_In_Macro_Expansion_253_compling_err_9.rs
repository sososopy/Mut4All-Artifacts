use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Insert_Doc_Comment_In_Macro_Expansion_253;

impl Mutator for Insert_Doc_Comment_In_Macro_Expansion_253 {
    fn name(&self) -> &str {
        "Insert_Doc_Comment_In_Macro_Expansion_253"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(macro_item) = item {
                if let syn::Macro::MacroRules(rules) = &mut macro_item.mac {
                    for rule in &mut rules.rules {
                        if let syn::MacroRulesPats::Single(pats) = &rule.pats {
                            if let syn::MacroRulesPat::Item(_) = pats.pat {
                                if let Some(ref mut expansion) = rule.body {
                                    if let Ok(expanded_item) = syn::parse2::<Item>(expansion.clone()) {
                                        if let Item::Fn(_) = expanded_item {
                                            let item_tokens = expanded_item.to_token_stream();
                                            let comment = quote! { /// };
                                            let new_expansion = quote! { #comment #item_tokens };
                                            *expansion = new_expansion;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}