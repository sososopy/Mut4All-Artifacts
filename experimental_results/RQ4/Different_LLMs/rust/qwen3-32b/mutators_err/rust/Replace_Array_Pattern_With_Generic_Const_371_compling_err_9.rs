use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprIf, ExprLet, ExprMatch, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    parse_quote,
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
                let block = &mut *func.block;
                for stmt in &mut block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Match(expr_match) = expr {
                            for arm in &mut expr_match.arms {
                                if let &Pat::Slice(_) = &*arm.pat {
                                    let new_pat = create_nested_pattern(&const_ident);
                                    arm.pat = Box::new(new_pat);
                                }
                            }
                        } else if let Expr::Let(expr_let) = expr {
                            if let &Pat::Slice(_) = &*expr_let.pat {
                                let new_pat = create_nested_pattern(&const_ident);
                                expr_let.pat = Box::new(new_pat);
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
    let inner_array: Pat = parse_quote! { [..] };
    let repeat_count: Pat = parse_quote! { _ };
    let first_arm: Pat = parse_quote! { [..; #repeat_count] };
    let len_expr: Expr = parse_quote! { #const_ident };
    let second_arm: Pat = parse_quote! { [..; #len_expr] };
    parse_quote! { #first_arm | #second_arm }
}