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

pub struct Modify_Trait_Bound_With_Const_Generics_133;

impl Mutator for Modify_Trait_Bound_With_Const_Generics_133 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_With_Const_Generics_133"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        for bound in &mut type_item.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                for segment in &mut trait_bound.path.segments {
                                    if let syn::PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                        for arg in &mut args.args {
                                            if let syn::GenericArgument::Const(expr) = arg {
                                                if let syn::Expr::Lit(expr_lit) = expr {
                                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                        let new_value = match lit_int.base10_parse::<i32>() {
                                                            Ok(1) => 0,
                                                            Ok(0) => 2,
                                                            _ => i32::MAX,
                                                        };
                                                        *expr = syn::Expr::Lit(syn::ExprLit {
                                                            attrs: Vec::new(),
                                                            lit: syn::Lit::Int(syn::LitInt::new(&new_value.to_string(), lit_int.span())),
                                                        });
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
    fn chain_of_thought(&self) -> &str {
        ""
    }
}