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

pub struct Replace_Const_Generic_With_Nonconst_Generic_7;

impl Mutator for Replace_Const_Generic_With_Nonconst_Generic_7 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Nonconst_Generic_7"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut found_const_generic = false;
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        if let Some((_, bounds)) = &mut type_item.bounds.iter_mut().next() {
                            for bound in bounds.iter_mut() {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    for segment in trait_bound.path.segments.iter_mut() {
                                        if segment.ident == "U" {
                                            if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                for arg in args.args.iter_mut() {
                                                    if let syn::GenericArgument::Const(expr) = arg {
                                                        if let syn::Expr::Lit(expr_lit) = expr {
                                                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                                if lit_int.base10_parse::<i32>().is_ok() {
                                                                    *arg = syn::GenericArgument::Type(parse_quote!(N));
                                                                    found_const_generic = true;
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
                if found_const_generic {
                    trait_item.generics.params.push(parse_quote!(const N: i32));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}