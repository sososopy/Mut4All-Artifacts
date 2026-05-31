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

pub struct Replace_Fn_Param_With_GAT_130;

impl Mutator for Replace_Fn_Param_With_GAT_130 {
    fn name(&self) -> &str {
        "Replace_Fn_Param_With_GAT_130"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut gats = Vec::new();
                for item_trait in &trait_item.items {
                    if let syn::TraitItem::Type(trait_type) = item_trait {
                        if let Some(lt_token) = trait_type.generics.lt_token {
                            if let Some(where_clause) = &trait_type.where_clause {
                                if !where_clause.predicates.is_empty() {
                                    gats.push(trait_type);
                                }
                            }
                        }
                    }
                }
                for item_trait in &mut trait_item.items {
                    if let syn::TraitItem::Fn(func) = item_trait {
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::ImplTrait(type_impl_trait) = &**pat_type.ty {
                                    if let Some(trait_bound) = type_impl_trait.bounds.iter().find_map(|bound| {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            Some(trait_bound)
                                        } else {
                                            None
                                        }
                                    }) {
                                        if trait_bound.path.is_ident("Fn") {
                                            if let Some(gat) = gats.first() {
                                                let gat_name = &gat.ident;
                                                let new_type = parse_quote!(Self::#gat_name<'_>);
                                                *pat_type.ty = Box::new(new_type);
                                                if let Some(where_clause) = &gat.where_clause {
                                                    let mut new_where_clause = func.sig.generics.where_clause.take().unwrap_or_else(|| {
                                                        syn::WhereClause {
                                                            where_token: Default::default(),
                                                            predicates: Punctuated::new(),
                                                        }
                                                    });
                                                    for pred in &where_clause.predicates {
                                                        new_where_clause.predicates.push(pred.clone());
                                                    }
                                                    func.sig.generics.where_clause = Some(new_where_clause);
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
        "The mutation operator replaces function parameters using `impl Fn` with a GAT from the same trait, adding necessary `where` clauses. This transformation introduces complex lifetime constraints and tests the compiler's ability to resolve GATs with nested lifetime relationships under opaque trait bounds."
    }
}