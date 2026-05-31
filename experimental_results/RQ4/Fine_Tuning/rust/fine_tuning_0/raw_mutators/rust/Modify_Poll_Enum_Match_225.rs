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

pub struct Modify_Poll_Enum_Match_225;

impl Mutator for Modify_Poll_Enum_Match_225 {
    fn name(&self) -> &str {
        "Modify_Poll_Enum_Match_225"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut found_poll_match = false;
                
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Match(expr_match)) = stmt {
                        if let Expr::Path(ExprPath { path, .. }) = &*expr_match.expr {
                            if path.is_ident("Poll") {
                                found_poll_match = true;
                                let mut has_custom_type = false;
                                
                                for arm in &mut expr_match.arms {
                                    if let Pat::TupleStruct(pat_tuple_struct) = &mut arm.pat {
                                        if let Some(first_segment) = pat_tuple_struct.path.segments.first() {
                                            if first_segment.ident == "Ready" {
                                                if let Some(inner_pat) = pat_tuple_struct.pat.elems.first_mut() {
                                                    if let Pat::TupleStruct(inner_pat_tuple_struct) = inner_pat {
                                                        if let Some(inner_segment) = inner_pat_tuple_struct.path.segments.first() {
                                                            if inner_segment.ident == "Ok" {
                                                                if let Some(inner_inner_pat) = inner_pat_tuple_struct.pat.elems.first_mut() {
                                                                    if let Pat::TupleStruct(inner_inner_pat_tuple_struct) = inner_inner_pat {
                                                                        if let Some(inner_inner_segment) = inner_inner_pat_tuple_struct.path.segments.first() {
                                                                            if inner_inner_segment.ident == "Some" {
                                                                                if !has_custom_type {
                                                                                    has_custom_type = true;
                                                                                    let custom_type: Item = parse_quote! {
                                                                                        enum CustomType {
                                                                                            Value(u32),
                                                                                        }
                                                                                    };
                                                                                    file.items.push(custom_type);
                                                                                }
                                                                                inner_inner_pat_tuple_struct.path = parse_quote!(CustomType::Value);
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
                    }
                }
                
                if found_poll_match {
                    func.block.stmts.push(parse_quote! {
                        enum CustomType {
                            Value(u32),
                        }
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}