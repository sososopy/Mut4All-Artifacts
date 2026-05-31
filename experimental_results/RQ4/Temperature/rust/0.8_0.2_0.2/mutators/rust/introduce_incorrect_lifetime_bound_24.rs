use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, File, Item, ItemFn, ItemImpl,
    WherePredicate,
};

use crate::mutator::Mutator;

pub struct Introduce_Incorrect_Lifetime_Bound_24;

impl Mutator for Introduce_Incorrect_Lifetime_Bound_24 {
    fn name(&self) -> &str {
        "Introduce_Incorrect_Lifetime_Bound_24"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    let mut new_predicates = where_clause.predicates.clone();
                    new_predicates.push(parse_quote!(for<'a> T: 'a));
                    where_clause.predicates = new_predicates;
                }
            }
            if let Item::Impl(item_impl) = item {
                if let Some(where_clause) = &mut item_impl.generics.where_clause {
                    let mut new_predicates = where_clause.predicates.clone();
                    new_predicates.push(parse_quote!(for<'a> T: 'a));
                    where_clause.predicates = new_predicates;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an incorrect lifetime bound `for<'a> T: 'a` into function and impl where clauses. This syntactically invalid constraint is designed to test the Rust compiler's handling of lifetime bounds, potentially exposing bugs in parsing or type-checking phases related to lifetime management."
    }
}