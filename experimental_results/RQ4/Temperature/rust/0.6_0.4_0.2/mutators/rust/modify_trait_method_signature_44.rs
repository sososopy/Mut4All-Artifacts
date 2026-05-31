use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, token::Comma, token::Paren,
    token::Plus, visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Method_Signature_44;

impl Mutator for Modify_Trait_Method_Signature_44 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Signature_44"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        
        // Collect all trait methods
        let mut trait_methods = vec![];
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Fn(method) = item {
                        trait_methods.push(method.clone());
                    }
                }
            }
        }
        
        // Randomly select a trait method to modify
        if let Some(selected_method) = trait_methods.choose(&mut rng) {
            for item in &mut file.items {
                if let Item::Trait(item_trait) = item {
                    for item in &mut item_trait.items {
                        if let TraitItem::Fn(method) = item {
                            if method.sig.ident == selected_method.sig.ident {
                                // Modify the number of parameters
                                let action = rng.gen_range(0..3);
                                match action {
                                    0 => {
                                        // Add a parameter
                                        let new_param: FnArg = parse_quote!(new_param: i32);
                                        method.sig.inputs.push(new_param);
                                    }
                                    1 => {
                                        // Remove a parameter if possible
                                        if !method.sig.inputs.is_empty() {
                                            method.sig.inputs.pop();
                                        }
                                    }
                                    2 => {
                                        // Alter a parameter type
                                        if let Some(FnArg::Typed(pat_type)) = method.sig.inputs.first_mut() {
                                            pat_type.ty = Box::new(parse_quote!(String));
                                        }
                                    }
                                    _ => {}
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator randomly selects a trait method and modifies its signature by either adding, removing, or altering parameters. This creates a mismatch between the trait declaration and its implementations, potentially triggering compiler errors related to method signature discrepancies."
    }
}