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

pub struct Modify_Struct_Field_To_Tuple_36;

impl Mutator for Modify_Struct_Field_To_Tuple_36 {
    fn name(&self) -> &str {
        "Modify_Struct_Field_To_Tuple_36"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if !item_struct.generics.params.is_empty() && !item_struct.fields.is_empty() {
                    if let Some(first_field) = item_struct.fields.iter_mut().next() {
                        if let Type::Path(TypePath { path, .. }) = &first_field.ty {
                            if let Some(segment) = path.segments.first() {
                                if item_struct.generics.params.iter().any(|param| {
                                    if let GenericParam::Type(type_param) = param {
                                        type_param.ident == segment.ident
                                    } else {
                                        false
                                    }
                                }) {
                                    let generic_ident = segment.ident.clone();
                                    first_field.ty = parse_quote!((#generic_ident, #generic_ident));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs with generic parameters and modifies one field to use a tuple of the generic type. It ensures consistency by adjusting the field type from `T` to `(T, T)`, maintaining the syntactic correctness of the struct."
    }
}