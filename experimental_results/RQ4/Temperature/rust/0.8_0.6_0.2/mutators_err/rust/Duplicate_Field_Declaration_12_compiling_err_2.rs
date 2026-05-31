use proc_macro2::Span;
use quote::quote;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, Fields, Item, ItemStruct, Type, File,
};

use crate::mutator::Mutator;

pub struct Duplicate_Field_Declaration_12;

impl Mutator for Duplicate_Field_Declaration_12 {
    fn name(&self) -> &str {
        "Duplicate_Field_Declaration_12"
    }
    
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Fields::Named(fields_named) = &mut item_struct.fields {
                    if let Some(field) = fields_named.named.first() {
                        if let Some(ident) = &field.ident {
                            // Duplicate the field declaration
                            let duplicate_field: syn::Field = parse_quote! {
                                #ident: #field.ty
                            };
                            fields_named.named.push(duplicate_field);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with named fields and duplicates one of the field declarations within the struct. This operation aims to introduce a field uniqueness error, testing the compiler's ability to handle and report duplicate field declarations within a single struct definition."
    }
}