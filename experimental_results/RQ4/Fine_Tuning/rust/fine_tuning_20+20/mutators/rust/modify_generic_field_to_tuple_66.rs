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

pub struct Modify_Generic_Field_To_Tuple_66;

impl Mutator for Modify_Generic_Field_To_Tuple_66 {
    fn name(&self) -> &str {
        "Modify_Generic_Field_To_Tuple_66"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if item_struct.generics.params.is_empty() || item_struct.fields.is_empty() {
                    continue;
                }
                let mut rng = thread_rng();
                let generic_indices: Vec<usize> = item_struct
                    .generics
                    .params
                    .iter()
                    .enumerate()
                    .filter_map(|(i, param)| {
                        if matches!(param, GenericParam::Type(_)) {
                            Some(i)
                        } else {
                            None
                        }
                    })
                    .collect();
                if generic_indices.is_empty() {
                    continue;
                }
                let chosen_generic_index = generic_indices.choose(&mut rng).unwrap();
                let chosen_generic_ident = match &item_struct.generics.params[*chosen_generic_index]
                {
                    GenericParam::Type(type_param) => type_param.ident.clone(),
                    _ => continue,
                };
                let mut field_indices = Vec::new();
                for (i, field) in item_struct.fields.iter().enumerate() {
                    if let Type::Path(type_path) = &field.ty {
                        if type_path
                            .path
                            .segments
                            .last()
                            .map_or(false, |seg| seg.ident == chosen_generic_ident)
                        {
                            field_indices.push(i);
                        }
                    }
                }
                if field_indices.is_empty() {
                    continue;
                }
                let chosen_field_index = field_indices.choose(&mut rng).unwrap();
                let new_type: Type = parse_quote!((#chosen_generic_ident, #chosen_generic_ident));
                if let syn::Fields::Named(fields_named) = &mut item_struct.fields {
                    fields_named.named[*chosen_field_index].ty = new_type;
                } else if let syn::Fields::Unnamed(fields_unnamed) = &mut item_struct.fields {
                    fields_unnamed.unnamed[*chosen_field_index].ty = new_type;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with generic parameters and at least one field. It selects a field whose type matches one of the generic parameters and transforms it into a tuple of that generic type. For example, a field of type `T` might be changed to `(T, T)`. This transformation increases the complexity of the struct's layout and tests the compiler's ability to handle changes in field types, particularly in terms of layout and size computations during constant evaluation."
    }
}