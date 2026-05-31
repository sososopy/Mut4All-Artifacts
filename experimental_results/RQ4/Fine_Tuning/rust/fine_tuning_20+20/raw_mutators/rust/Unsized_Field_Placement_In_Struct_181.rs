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

pub struct Unsized_Field_Placement_In_Struct_181;

impl Mutator for Unsized_Field_Placement_In_Struct_181 {
    fn name(&self) -> &str {
        "Unsized_Field_Placement_In_Struct_181"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Fields::Named(fields_named) = &mut item_struct.fields {
                    let mut sized_indices = Vec::new();
                    for (i, field) in fields_named.named.iter().enumerate() {
                        if let Type::Path(type_path) = &field.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident != "str" {
                                    sized_indices.push(i);
                                }
                            }
                        }
                    }
                    if sized_indices.len() <= 1 {
                        continue;
                    }
                    let mut rng = thread_rng();
                    let unsized_index = *sized_indices.choose(&mut rng).unwrap();
                    let unsized_type: Type = parse_quote!([u8]);
                    fields_named.named[unsized_index].ty = unsized_type;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets struct definitions with multiple fields by converting a randomly selected non-terminal sized field to an unsized type, such as a slice. This transformation exploits Rust's requirement that unsized fields must be the last field unless behind a pointer, potentially triggering layout errors or compiler crashes due to violated struct field constraints."
    }
}