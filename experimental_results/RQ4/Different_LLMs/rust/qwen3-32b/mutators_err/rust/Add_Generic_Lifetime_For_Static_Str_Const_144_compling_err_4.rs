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

pub struct Add_Generic_Lifetime_For_Static_Str_Const_144;

impl Mutator for Add_Generic_Lifetime_For_Static_Str_Const_144 {
    fn name(&self) -> &str {
        "Add_Generic_Lifetime_For_Static_Str_Const_144"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(_) = func.sig.constness {
                    let mut has_static_str = false;
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Pat::Ident(ident) = &*pat_type.pat {
                                if let Some(attr) = ident.attrs.iter().find(|a| a.path().is_ident("const")) {
                                    if let Ok(expr) = syn::parse2::<Expr>(attr.tokens.clone()) {
                                        if let Expr::Lit(expr_lit) = expr {
                                            if let Type::Reference(type_ref) = &*pat_type.ty {
                                                if let Type::Path(type_path) = &*type_ref.elem {
                                                    if type_path.path.is_ident("str") {
                                                        if let Some(lifetime) = &type_ref.lifetime {
                                                            if lifetime.ident == "static" {
                                                                has_static_str = true;
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
                    }
                    if has_static_str {
                        let generics = &mut func.sig.generics;
                        let has_a = generics.params.iter().any(|param| {
                            if let GenericParam::Lifetime(lifetime_param) = param {
                                lifetime_param.lifetime.ident == "a"
                            } else {
                                false
                            }
                        });
                        if !has_a {
                            generics.params.insert(0, parse_quote!(lifetime 'a));
                        }
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Pat::Ident(ident) = &*pat_type.pat {
                                    if let Some(attr) = ident.attrs.iter().find(|a| a.path().is_ident("const")) {
                                        if let Ok(expr) = syn::parse2::<Expr>(attr.tokens.clone()) {
                                            if let Expr::Lit(expr_lit) = expr {
                                                if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                                    if let Type::Path(type_path) = &*type_ref.elem {
                                                        if type_path.path.is_ident("str") {
                                                            if let Some(lifetime) = &type_ref.lifetime {
                                                                if lifetime.ident == "static" {
                                                                    type_ref.lifetime = Some(Lifetime::new(&"'a", Span::call_site()));
                                                                    let where_clause = &mut generics.where_clause;
                                                                    if where_clause.is_none() {
                                                                        *where_clause = Some(WhereClause {
                                                                            where_token: token::Where::default(),
                                                                            predicates: Punctuated::new(),
                                                                        });
                                                                    }
                                                                    let where_clause = where_clause.as_mut().unwrap();
                                                                    let param_name = ident.ident.clone();
                                                                    let predicate = parse_quote!( [(); #param_name.len()] : {} );
                                                                    where_clause.predicates.push_value(predicate);
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
                        }
                    }
                }
            }
            if let syn::Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                let mut has_static_str = false;
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Reference(type_ref) = &*const_param.ty {
                            if let Type::Path(type_path) = &*type_ref.elem {
                                if type_path.path.is_ident("str") {
                                    if let Some(lifetime) = &type_ref.lifetime {
                                        if lifetime.ident == "static" {
                                            has_static_str = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if has_static_str {
                    let has_a = generics.params.iter().any(|param| {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            lifetime_param.lifetime.ident == "a"
                        } else {
                            false
                        }
                    });
                    if !has_a {
                        generics.params.insert(0, parse_quote!(lifetime 'a));
                    }
                    for param in &mut generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Reference(type_ref) = &mut *const_param.ty {
                                if let Type::Path(type_path) = &*type_ref.elem {
                                    if type_path.path.is_ident("str") {
                                        if let Some(lifetime) = &type_ref.lifetime {
                                            if lifetime.ident == "static" {
                                                type_ref.lifetime = Some(Lifetime::new(&"'a", Span::call_site()));
                                                let where_clause = &mut generics.where_clause;
                                                if where_clause.is_none() {
                                                    *where_clause = Some(WhereClause {
                                                        where_token: token::Where::default(),
                                                        predicates: Punctuated::new(),
                                                    });
                                                }
                                                let where_clause = where_clause.as_mut().unwrap();
                                                let param_name = const_param.ident.clone();
                                                let predicate = parse_quote!( [(); #param_name.len()] : {} );
                                                where_clause.predicates.push_value(predicate);
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}