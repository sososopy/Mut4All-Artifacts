use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, Fields, FieldsNamed, Item, ItemStruct,
    Type, Ident, File, 
};

use crate::mutator::Mutator;

pub struct Add_Recursive_Type_Alias_75;

impl Mutator for Add_Recursive_Type_Alias_75 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_75"
    }
    fn mutate(&self, file: &mut File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let struct_name = &item_struct.ident;
                let type_alias: Item = parse_quote! {
                    type RecursiveAlias = Box<#struct_name>;
                };
                new_items.push(type_alias);

                if let Fields::Named(FieldsNamed { ref mut named, .. }) = item_struct.fields {
                    let recursive_field: syn::Field = parse_quote! {
                        pub recursive: RecursiveAlias,
                    };
                    named.push(recursive_field);
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias within struct definitions. By adding a field that references the struct itself through a type alias, it aims to provoke deep recursion issues, type resolution failures, or infinite loops in the compiler's type-checking and resolution phases. This transformation tests the compiler's ability to handle recursive types and cyclic dependencies."
    }
}