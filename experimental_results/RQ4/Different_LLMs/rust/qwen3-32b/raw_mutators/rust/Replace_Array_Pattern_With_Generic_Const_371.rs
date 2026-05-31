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

pub struct Replace_Array_Pattern_With_Generic_Const_371;

impl Mutator for Replace_Array_Pattern_With_Generic_Const_371 {
    fn name(&self) -> &str {
        "Replace_Array_Pattern_With_Generic_Const_371"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let const_generics: Vec<&syn::ConstParam> = func.sig.generics.params.iter()
                    .filter_map(|param| if let syn::GenericParam::Const(c) = param { Some(c) } else { None })
                    .collect();
                if const_generics.is_empty() {
                    continue;
                }
                let const_ident = const_generics.first().unwrap().ident.clone();
                if let Some(block) = &mut func.block {
                    for stmt in &mut block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Match(expr_match) = &mut **expr {
                                for arm in &mut expr_match.arms {
                                    if let Pat::Slice(_) = &arm.pat {
                                        let new_pat = create_nested_pattern(&const_ident);
                                        arm.pat = new_pat;
                                    } else if let Pat::Array(pat_array) = &arm.pat {
                                        if pat_array.elems.len() > 0 && pat_array.suffix.is_empty() && pat_array.repeated.is_none() {
                                            let new_pat = create_nested_pattern(&const_ident);
                                            arm.pat = new_pat;
                                        }
                                    }
                                }
                            } else if let Expr::If(expr_if) = &mut **expr {
                                if let Some((pat, _)) = &mut expr_if.let_else {
                                    if let Pat::Slice(_) = pat {
                                        let new_pat = create_nested_pattern(&const_ident);
                                        *pat = Box::new(new_pat);
                                    } else if let Pat::Array(pat_array) = pat {
                                        if pat_array.elems.len() > 0 && pat_array.suffix.is_empty() && pat_array.repeated.is_none() {
                                            let new_pat = create_nested_pattern(&const_ident);
                                            *pat = Box::new(new_pat);
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
        "The mutation operator replaces structurally simple array or slice patterns in match or if-let expressions with nested array patterns involving a generic const expression from the function's const generics. This forces the compiler to evaluate nested const expressions in pattern contexts, exposing mismatches between expected/actual const types during MIR construction and borrow checking."
    }
}

fn create_nested_pattern(const_ident: &Ident) -> Pat {
    let inner_array = Pat::Array(Box::new(PatArray {
        bracket_token: token::Bracket::default(),
        elems: Punctuated::new(),
        repeated: None,
        attrs: vec![],
        semi_token: None,
    }));
    let repeat_count = Pat::Wild(PatWild {
        attrs: vec![],
        underscore: Span::call_site().into(),
        subpat: None,
    });
    let first_arm = Pat::Array(Box::new(PatArray {
        bracket_token: token::Bracket::default(),
        elems: Punctuated::from_iter(vec![inner_array.clone()]),
        repeated: Some(Box::new((token::At::default(), repeat_count))),
        attrs: vec![],
        semi_token: None,
    }));
    let len_expr = parse_quote! { #const_ident };
    let second_arm = Pat::Array(Box::new(PatArray {
        bracket_token: token::Bracket::default(),
        elems: Punctuated::from_iter(vec![inner_array]),
        len: Some(Box::new(len_expr)),
        attrs: vec![],
        semi_token: None,
    }));
    let or_pat = Pat::Or(PatOr {
        attrs: vec![],
        leading_vert: token::Or::default(),
        cases: Punctuated::from_iter(vec![first_arm, second_arm]),
    });
    or_pat
}