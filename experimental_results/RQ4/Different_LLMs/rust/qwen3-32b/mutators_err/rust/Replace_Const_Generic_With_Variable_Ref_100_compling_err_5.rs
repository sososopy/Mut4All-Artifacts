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
    token::{Paren, Plus, Eq},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Const_Generic_With_Variable_Ref_100;

impl Mutator for Replace_Const_Generic_With_Variable_Ref_100 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Variable_Ref_100"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let block = &mut func.block;
                let mut vars = HashSet::new();
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Pat::Ident(pat_ident) = &*pat_type.pat {
                            vars.insert(pat_ident.ident.to_string());
                        }
                    }
                }
                for stmt in &block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Ident(pat_ident) = &local.pat {
                            vars.insert(pat_ident.ident.to_string());
                        }
                    }
                }
                if vars.is_empty() {
                    let new_var = "x";
                    let new_local = Local {
                        attrs: vec![],
                        let_token: Default::default(),
                        pat: Pat::Ident(PatIdent {
                            attrs: vec![],
                            by_ref: None,
                            mutability: None,
                            ident: Ident::new("x", Span::call_site()),
                            subpat: None,
                        }),
                        init: Some((token::Eq::default(), Box::new(Expr::Verbatim(quote! { 0 })))),
                        semi_token: Default::default(),
                    };
                    block.stmts.insert(0, Stmt::Local(new_local));
                    vars.insert("x".to_string());
                }
                let mut found_expr = None;
                for param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                for segment in &mut trait_bound.path.segments {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Const(expr) = arg {
                                                found_expr = Some((expr, vars.clone()));
                                                break;
                                            }
                                        }
                                        if found_expr.is_some() {
                                            break;
                                        }
                                    }
                                }
                                if found_expr.is_some() {
                                    break;
                                }
                            }
                        }
                        if found_expr.is_some() {
                            break;
                        }
                    }
                }
                if found_expr.is_none() {
                    if let Some(where_clause) = &mut func.sig.generics.where_clause {
                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(type_pred) = predicate {
                                for bound in &mut type_pred.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        for segment in &mut trait_bound.path.segments {
                                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                for arg in &mut args.args {
                                                    if let GenericArgument::Const(expr) = arg {
                                                        found_expr = Some((expr, vars.clone()));
                                                        break;
                                                    }
                                                }
                                                if found_expr.is_some() {
                                                    break;
                                                }
                                            }
                                        }
                                        if found_expr.is_some() {
                                            break;
                                        }
                                    }
                                }
                                if found_expr.is_some() {
                                    break;
                                }
                            }
                        }
                    }
                }
                if let Some((expr, vars)) = found_expr {
                    let var_name = vars.iter().next().unwrap();
                    let new_expr = parse_quote! { #var_name };
                    *expr = new_expr;
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let block = &mut func.block;
                        let mut vars = HashSet::new();
                        for input in &func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                    vars.insert(pat_ident.ident.to_string());
                                }
                            }
                        }
                        for stmt in &block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Pat::Ident(pat_ident) = &local.pat {
                                    vars.insert(pat_ident.ident.to_string());
                                }
                            }
                        }
                        if vars.is_empty() {
                            let new_var = "x";
                            let new_local = Local {
                                attrs: vec![],
                                let_token: Default::default(),
                                pat: Pat::Ident(PatIdent {
                                    attrs: vec![],
                                    by_ref: None,
                                    mutability: None,
                                    ident: Ident::new("x", Span::call_site()),
                                    subpat: None,
                                }),
                                init: Some((token::Eq::default(), Box::new(Expr::Verbatim(quote! { 0 })))),
                                semi_token: Default::default(),
                            };
                            block.stmts.insert(0, Stmt::Local(new_local));
                            vars.insert("x".to_string());
                        }
                        let mut found_expr = None;
                        for param in &mut func.sig.generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                for bound in &mut type_param.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        for segment in &mut trait_bound.path.segments {
                                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                for arg in &mut args.args {
                                                    if let GenericArgument::Const(expr) = arg {
                                                        found_expr = Some((expr, vars.clone()));
                                                        break;
                                                    }
                                                }
                                                if found_expr.is_some() {
                                                    break;
                                                }
                                            }
                                        }
                                        if found_expr.is_some() {
                                            break;
                                        }
                                    }
                                }
                                if found_expr.is_some() {
                                    break;
                                }
                            }
                        }
                        if found_expr.is_none() {
                            if let Some(where_clause) = &mut func.sig.generics.where_clause {
                                for predicate in &mut where_clause.predicates {
                                    if let syn::WherePredicate::Type(type_pred) = predicate {
                                        for bound in &mut type_pred.bounds {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                for segment in &mut trait_bound.path.segments {
                                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                        for arg in &mut args.args {
                                                            if let GenericArgument::Const(expr) = arg {
                                                                found_expr = Some((expr, vars.clone()));
                                                                break;
                                                            }
                                                        }
                                                        if found_expr.is_some() {
                                                            break;
                                                        }
                                                    }
                                                }
                                                if found_expr.is_some() {
                                                    break;
                                                }
                                            }
                                        }
                                        if found_expr.is_some() {
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if let Some((expr, vars)) = found_expr {
                            let var_name = vars.iter().next().unwrap();
                            let new_expr = parse_quote! { #var_name };
                            *expr = new_expr;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a const generic expression (e.g., { expr }) within a trait bound with a reference to a non-const variable declared in the same function or scope. This transforms the const generic into an unevaluated expression, potentially exposing ICEs in the compiler's evaluation logic when it incorrectly assumes the value is known at compile time."
    }
}.