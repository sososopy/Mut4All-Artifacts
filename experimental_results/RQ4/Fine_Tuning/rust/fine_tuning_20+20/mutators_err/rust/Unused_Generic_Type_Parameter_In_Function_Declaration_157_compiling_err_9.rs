use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
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

pub struct Unused_Generic_Type_Parameter_In_Function_Declaration_157;

impl Mutator for Unused_Generic_Type_Parameter_In_Function_Declaration_157 {
    fn name(&self) -> &str {
        "Unused_Generic_Type_Parameter_In_Function_Declaration_157"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.generics.params.is_empty() {
                    continue;
                }
                let mut existing_idents: HashSet<String> = HashSet::new();
                for param in &item_fn.sig.generics.params {
                    match param {
                        GenericParam::Type(type_param) => {
                            existing_idents.insert(type_param.ident.to_string());
                        }
                        GenericParam::Lifetime(lifetime_def) => {
                            existing_idents.insert(lifetime_def.lifetime.ident.to_string());
                        }
                        GenericParam::Const(const_param) => {
                            existing_idents.insert(const_param.ident.to_string());
                        }
                    }
                }
                let mut new_ident = String::new();
                for i in 0.. {
                    let candidate = format!("U{}", i);
                    if !existing_idents.contains(&candidate) {
                        new_ident = candidate;
                        break;
                    }
                }
                let new_generic: GenericParam = parse_quote! { #new_ident };
                item_fn.sig.generics.params.push(new_generic);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with existing generic parameters and adds a new unused generic type parameter to their signature. By introducing an unused type parameter, it tests the compiler's handling of generic parameter resolution, potentially exposing issues in type inference, unused parameter warnings, or internal compiler logic related to generics."
    }
}