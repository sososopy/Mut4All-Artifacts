use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use rand::prelude::IteratorRandom;
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Change_Struct_Field_Type_68;

impl Mutator for Change_Struct_Field_Type_68 {
    fn name(&self) -> &str {
        "Change_Struct_Field_Type_68"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut all_types = HashSet::new();

        // Collect all types present in the file
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                for field in &item_struct.fields {
                    if let Type::Path(type_path) = &field.ty {
                        if let Some(segment) = type_path.path.segments.last() {
                            all_types.insert(segment.ident.to_string());
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(field) = item_struct.fields.iter_mut().choose(&mut rng) {
                    if let Type::Path(type_path) = &mut field.ty {
                        if let Some(segment) = type_path.path.segments.last_mut() {
                            let current_type = segment.ident.to_string();
                            let alternative_types: Vec<_> = all_types
                                .iter()
                                .filter(|&&t| t != current_type)
                                .collect();

                            if let Some(new_type) = alternative_types.choose(&mut rng) {
                                segment.ident = Ident::new(new_type, Span::call_site());
                            } else {
                                segment.ident = Ident::new("i32", Span::call_site());
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies a struct within the code and changes one of its field types to another existing type within the program. If no alternative type is available, it defaults to a primitive type like `i32`. The goal is to test the compiler's handling of type changes and ensure type consistency throughout the program."
    }
}