use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Duplicate_Field_Mutation_5;

impl Mutator for Duplicate_Field_Mutation_5 {
    fn name(&self) -> &str {
        "Duplicate_Field_Mutation_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut field_names = HashSet::new();
                let mut duplicate_fields = Vec::new();

                if let syn::Fields::Named(fields_named) = &item_struct.fields {
                    for field in &fields_named.named {
                        if let Some(ident) = &field.ident {
                            if !field_names.insert(ident.to_string()) {
                                duplicate_fields.push(ident.clone());
                            }
                        }
                    }
                }

                if !duplicate_fields.is_empty() {
                    if let syn::Fields::Named(fields_named) = &mut item_struct.fields {
                        for field in &mut fields_named.named {
                            if let Some(ident) = &field.ident {
                                if duplicate_fields.contains(ident) {
                                    let new_ident = Ident::new(
                                        &format!("{}_renamed", ident),
                                        ident.span(),
                                    );
                                    duplicate_fields.retain(|x| x != ident);
                                    field.ident = Some(new_ident);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs with duplicate field names but different types. It renames the duplicate fields to ensure unique identifiers, thereby testing the compiler's handling of struct field name uniqueness and type mismatches."
    }
}