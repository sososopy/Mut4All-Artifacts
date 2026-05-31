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

pub struct Add_Unused_Generic_Params_210;

impl Mutator for Add_Unused_Generic_Params_210 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Params_210"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                generics.params.push(parse_quote!(T));
                generics.params.push(parse_quote!(U));
                if generics.where_clause.is_none() {
                    generics.where_clause = Some(parse_quote!(where T: std::fmt::Debug, U: std::fmt::Display));
                } else {
                    if let Some(where_clause) = &mut generics.where_clause {
                        where_clause.predicates.push(parse_quote!(T: std::fmt::Debug));
                        where_clause.predicates.push(parse_quote!(U: std::fmt::Display));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let generics = &mut func.sig.generics;
                        generics.params.push(parse_quote!(T));
                        generics.params.push(parse_quote!(U));
                        if generics.where_clause.is_none() {
                            generics.where_clause = Some(parse_quote!(where T: std::fmt::Debug, U: std::fmt::Display));
                        } else {
                            if let Some(where_clause) = &mut generics.where_clause {
                                where_clause.predicates.push(parse_quote!(T: std::fmt::Debug));
                                where_clause.predicates.push(parse_quote!(U: std::fmt::Display));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused generic parameters `T` and `U` with `Debug` and `Display` trait bounds to non-main function and impl signatures. This transformation introduces potentially redundant generic constraints, testing the compiler's handling of unused generics and trait bounds. It aims to provoke issues in type resolution and generic parameter management."
    }
}