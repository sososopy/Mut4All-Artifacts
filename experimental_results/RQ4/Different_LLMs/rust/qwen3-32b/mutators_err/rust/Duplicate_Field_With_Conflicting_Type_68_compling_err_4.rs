use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashMap, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Duplicate_Field_With_Conflicting_Type_68;

impl Mutator for Duplicate_Field_With_Conflicting_Type_68 {
    fn name(&self) -> &str {
        "Duplicate_Field_With_Conflicting_Type_68"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_structs = HashMap::new();
        // First pass: modify structs
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let syn::Fields::Named(data_struct) = &mut item_struct.fields {
                    if !data_struct.named.is_empty() {
                        // Select the first field
                        if let Some(first_field) = data_struct.named.iter().next() {
                            // Check if the first field has an ident (not a tuple struct)
                            if let Some(field_name) = &first_field.ident {
                                // Create a new field with the same name but unit type
                                let new_field = syn::Field {
                                    attrs: vec![],
                                    vis: syn::Visibility::Inherited,
                                    mutability: syn::FieldMutability::None,
                                    ident: Some(field_name.clone()),
                                    colon_token: Some(token::Colon::default()),
                                    ty: syn::Type::Tuple(syn::TypeTuple {
                                        paren_token: Default::default(),
                                        elems: Punctuated::new(),
                                    }),
                                };
                                data_struct.named.push(new_field);
                                // Record the struct name and duplicated field name
                                let struct_name = item_struct.ident.to_string();
                                modified_structs.insert(struct_name, field_name.to_string());
                            }
                        }
                    }
                }
            }
        }
        // Second pass: modify static variables
        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                if let Type::Path(type_path) = &*static_item.ty {
                    let struct_name = type_path.path.segments.last().unwrap().ident.to_string();
                    if let Some(duplicated_field) = modified_structs.get(&struct_name) {
                        if let Expr::Struct(expr_struct) = &mut *static_item.expr {
                            for field in &mut expr_struct.fields {
                                if let syn::Member::Named(ident) = field.member {
                                    if ident == duplicated_field.as_str() {
                                        // Replace the field's value with unit
                                        field.expr = Expr::Tuple(parse_quote!( ()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces duplicate struct fields with conflicting types and modifies static initializers to omit one field, creating type mismatches. This forces the compiler to resolve ambiguous field types and invalid initializations, testing its ability to handle struct field duplication and incomplete initializations."
    }
}