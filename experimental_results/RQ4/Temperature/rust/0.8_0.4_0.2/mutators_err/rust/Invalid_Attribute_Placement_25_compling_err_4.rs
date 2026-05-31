use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, Expr, File, Item, ItemFn, Stmt,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Invalid_Attribute_Placement_25;

impl Mutator for Invalid_Attribute_Placement_25 {
    fn name(&self) -> &str {
        "Invalid_Attribute_Placement_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let invalid_attribute: Stmt = parse_quote! {
                    #[some_invalid_attribute]
                    ()
                };
                func.block.stmts.insert(0, invalid_attribute);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}