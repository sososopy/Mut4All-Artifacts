use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Alter_Generic_Constraints_In_Trait_Impls_46;

impl Mutator for Alter_Generic_Constraints_In_Trait_Impls_46 {
    fn name(&self) -> &str {
        "Alter_Generic_Constraints_In_Trait_Impls_46"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, _, ref mut generics)) = &mut item_impl.generics.split_for_impl() {
                    if generics.params.is_empty() {
                        continue;
                    }
                    let mut rng = thread_rng();
                    let mutation_choice = rng.gen_range(0..3);

                    match mutation_choice {
                        0 => {
                            // Remove a constraint
                            if let Some(GenericParam::Type(type_param)) = generics.params.first_mut() {
                                if !type_param.bounds.is_empty() {
                                    type_param.bounds.clear();
                                }
                            }
                        }
                        1 => {
                            // Add a new constraint
                            if let Some(GenericParam::Type(type_param)) = generics.params.first_mut() {
                                type_param.bounds.push(parse_quote!(Clone));
                            }
                        }
                        2 => {
                            // Modify an existing constraint
                            if let Some(GenericParam::Type(type_param)) = generics.params.first_mut() {
                                if !type_param.bounds.is_empty() {
                                    type_param.bounds.clear();
                                    type_param.bounds.push(parse_quote!(Debug));
                                }
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}