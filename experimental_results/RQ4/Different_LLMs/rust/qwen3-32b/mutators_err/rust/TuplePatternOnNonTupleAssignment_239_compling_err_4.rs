use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path,
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

pub struct TuplePatternOnNonTupleAssignment_239;

impl Mutator for TuplePatternOnNonTupleAssignment_239 {
    fn name(&self) -> &str {
        "TuplePatternOnNonTupleAssignment_239"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let syn::Stmt::Local(local) = stmt {
                        if let syn::Pat::Ident(pat_ident) = &*local.pat {
                            let new_pat = syn::Pat::Tuple(syn::PatTuple {
                                attrs: Vec::new(),
                                paren_token: syn::token::Paren::default(),
                                elems: {
                                    let mut elems = syn::punctuated::Punctuated::new();
                                    let wild = syn::Pat::Wild(syn::PatWild {
                                        attrs: Vec::new(),
                                        underscore_token: syn::token::Underscore::default(),
                                    });
                                    elems.push(wild);
                                    let new_ident = syn::Ident::new(
                                        &format!("{}_part", pat_ident.ident.to_string()),
                                        pat_ident.ident.span(),
                                    );
                                    let new_pat_ident = syn::Pat::Ident(syn::PatIdent {
                                        attrs: Vec::new(),
                                        by_ref: pat_ident.by_ref,
                                        mutability: pat_ident.mutability,
                                        ident: new_ident,
                                        subpat: pat_ident.subpat.clone(),
                                    });
                                    elems.push(new_pat_ident);
                                    elems
                                },
                            });
                            local.pat = new_pat;
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