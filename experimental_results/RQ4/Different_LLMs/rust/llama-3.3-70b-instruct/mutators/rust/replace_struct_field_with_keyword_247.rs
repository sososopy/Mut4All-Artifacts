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

pub struct Replace_Struct_Field_With_Keyword_247;

impl Mutator for Replace_Struct_Field_With_Keyword_247 {
    fn name(&self) -> &str {
        "Replace_Struct_Field_With_Keyword_247"
    }
    fn mutate(&self, file: &mut syn::File) {
        let keywords = vec!["Self", "super", "self", "crate"];
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Some(ident) = &mut field.ident {
                        *ident = Ident::new(
                            keywords.choose(&mut thread_rng()).unwrap(),
                            Spanned::span(ident),
                        );
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Struct(ref mut pat_struct) = local.pat {
                            for field in &mut pat_struct.fields {
                                if let Member::Named(ident) = &mut field.member {
                                    *ident = Ident::new(
                                        keywords.choose(&mut thread_rng()).unwrap(),
                                        Spanned::span(ident),
                                    );
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces struct field names with Rust keywords. This transformation tests the compiler's ability to handle keywords in struct field names and can reveal bugs in the compiler's handling of keywords in pattern matching."
    }
}