use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    str::FromStr,
};
use syn::{
    parse_quote,
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

pub struct Modify_Const_Generic_Bounds_In_Function_Definitions_102;

impl Mutator for Modify_Const_Generic_Bounds_In_Function_Definitions_102 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Bounds_In_Function_Definitions_102"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut const_params = Vec::new();
                let mut other_params = Vec::new();
                for param in &func.sig.generics.params {
                    match param {
                        syn::GenericParam::Const(_) => const_params.push(param.clone()),
                        _ => other_params.push(param.clone()),
                    }
                }
                if const_params.len() < 2 {
                    return;
                }
                let new_where_clause: syn::WhereClause = parse_quote! {
                    where
                        #(#const_params): std::ops::Add<Output = usize>,
                        [(); #(#const_params)+*]: Sized,
                };
                func.sig.generics.where_clause = Some(new_where_clause);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with const generics by transforming their where clauses to enforce a complex constraint involving the addition of all const generic parameters. It replaces the existing where clause with one that requires the sum of these const parameters to be a valid array size, thereby introducing a challenging constraint that tests the compiler's const evaluation and type checking capabilities."
    }
}