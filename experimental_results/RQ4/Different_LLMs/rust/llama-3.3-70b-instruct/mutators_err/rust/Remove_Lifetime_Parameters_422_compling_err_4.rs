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

pub struct Remove_Lifetime_Parameters_422;

impl Mutator for Remove_Lifetime_Parameters_422 {
    fn name(&self) -> &str {
        "Remove_Lifetime_Parameters_422"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                if let Some(where_clause) = &mut generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = predicate_type.bounded_ty {
                                if let Some(lifetimes) = type_path.path.segments.last().unwrap().arguments {
                                    if let PathArguments::AngleBracketed(angle_bracketed) = lifetimes {
                                        for arg in &mut angle_bracketed.args {
                                            if let GenericArgument::Lifetime(_) = arg {
                                                *arg = GenericArgument::Const(parse_quote!(0));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                generics.params = generics.params.into_iter().filter(|param| !matches!(param, GenericParam::Lifetime(_))).collect();
            } else if let syn::Item::Trait(item_trait) = item {
                let generics = &mut item_trait.generics;
                if let Some(where_clause) = &mut generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = predicate_type.bounded_ty {
                                if let Some(lifetimes) = type_path.path.segments.last().unwrap().arguments {
                                    if let PathArguments::AngleBracketed(angle_bracketed) = lifetimes {
                                        for arg in &mut angle_bracketed.args {
                                            if let GenericArgument::Lifetime(_) = arg {
                                                *arg = GenericArgument::Const(parse_quote!(0));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                generics.params = generics.params.into_iter().filter(|param| !matches!(param, GenericParam::Lifetime(_))).collect();
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes lifetime parameters from structs and traits. It checks if the removal of lifetime parameters leads to a compiler error or panic, which could indicate a bug in the Rust compiler's handling of lifetime parameters."
    }
}