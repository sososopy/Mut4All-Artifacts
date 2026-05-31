use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Alter_Union_Field_Types_16;

impl Mutator for Alter_Union_Field_Types_16 {
    fn name(&self) -> &str {
        "Alter_Union_Field_Types_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                let new_fields = vec![
                    parse_quote!(b: u16),
                    parse_quote!(c: f64),
                ];

                if let Fields::Named(fields_named) = &mut item_union.fields {
                    for field in new_fields {
                        fields_named.named.push(field);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets union definitions, adding fields of different types (u16 and f64) to existing unions. This transformation aims to test the compiler's handling of unions with mixed type fields, potentially exposing alignment and layout issues."
    }
}