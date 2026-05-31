use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
    WherePredicate, WhereClause,
};

use crate::mutator::Mutator;

pub struct Add_Variadic_Lifetime_Constraint_234;

impl Mutator for Add_Variadic_Lifetime_Constraint_234 {
    fn name(&self) -> &str {
        "Add_Variadic_Lifetime_Constraint_234"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() && !func.sig.generics.params.is_empty() {
                    let where_clause = func.sig.generics.where_clause.get_or_insert_with(|| WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::new(),
                    });

                    let variadic_constraint: WherePredicate = parse_quote! {
                        for<'a> T: 'a
                    };

                    where_clause.predicates.push(variadic_constraint);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}