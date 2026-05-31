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

pub struct Replace_GAT_Lifetime_Bound_With_Static_130;

impl Mutator for Replace_GAT_Lifetime_Bound_With_Static_130 {
    fn name(&self) -> &str {
        "Replace_GAT_Lifetime_Bound_With_Static_130"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(type_item) = impl_item {
                        if let Some(where_clause) = &mut type_item.generics.where_clause {
                            let mut predicates = Punctuated::new();
                            predicates.push(syn::WherePredicate::Lifetime(syn::PredicateLifetime {
                                lifetime: Lifetime::new("'static", Span::call_site()),
                                colon_token: Default::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(Lifetime::new("'static", Span::call_site()));
                                    bounds
                                },
                            }));
                            where_clause.predicates = predicates;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic associated type definitions within impl blocks that have where clauses containing lifetime bounds. It replaces the entire where clause with a placeholder bound 'static: 'static, which always holds but may mismatch the trait's required bounds. This transformation aims to expose bugs in bound checking, error reporting, and the interaction between GATs, lifetime bounds, and higher-ranked trait bounds in function signatures."
    }
}