use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Union_Field_With_Associated_Type_10;

impl Mutator for Union_Field_With_Associated_Type_10 {
    fn name(&self) -> &str {
        "Union_Field_With_Associated_Type_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_assoc_type = std::collections::HashMap::new();
        
        // Step 1: Collect structs and their associated types
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let syn::Type::Path(type_path) = &field.ty {
                        for segment in &type_path.path.segments {
                            if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                                for arg in &args.args {
                                    if let syn::GenericArgument::Constraint(_) = arg {
                                        // Found a constraint, check if there's an associated type
                                        if let Some(second_arg) = args.args.get(1) {
                                            if let syn::GenericArgument::Type(ty) = second_arg {
                                                let struct_name = item_struct.ident.clone();
                                                struct_assoc_type.insert(struct_name, ty.clone());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Step 2: Process unions and replace struct fields with associated types
        for item in &mut file.items {
            if let syn::Item::Union(union) = item {
                for field in &mut union.fields.named {
                    if let syn::Type::Path(type_path) = &field.ty {
                        if let Some(ident) = type_path.path.get_ident() {
                            if let Some(assoc_type) = struct_assoc_type.get(ident) {
                                // Replace the field's type with the associated type
                                field.ty = assoc_type.clone();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a union field that references a struct with a field of the struct's associated type directly from the trait. This transformation bypasses the struct intermediary, directly using the associated type. It tests the compiler's ability to resolve associated types in union contexts and ensures trait implementations are correctly applied when struct fields are replaced."
    }
}