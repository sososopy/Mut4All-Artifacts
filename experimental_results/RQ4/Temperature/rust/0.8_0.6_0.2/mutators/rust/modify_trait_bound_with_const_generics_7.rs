use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
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

pub struct Modify_Trait_Bound_With_Const_Generics_7;

impl Mutator for Modify_Trait_Bound_With_Const_Generics_7 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_With_Const_Generics_7"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Type(type_item) = item {
                        let bounds = &mut type_item.bounds;
                        for bound in bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                                    if last_segment.ident == "B" {
                                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    if let Expr::Lit(expr_lit) = expr {
                                                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                            let new_value = match lit_int.base10_parse::<i32>() {
                                                                Ok(val) => val + 1,
                                                                Err(_) => 1,
                                                            };
                                                            expr_lit.lit = syn::Lit::Int(syn::LitInt::new(&new_value.to_string(), lit_int.span()));
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
    fn chain_of_thought(&self) -> &str {
        ""
    }
}