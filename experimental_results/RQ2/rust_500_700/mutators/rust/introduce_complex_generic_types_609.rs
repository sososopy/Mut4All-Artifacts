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

pub struct Introduce_Complex_Generic_Types_609;

impl Mutator for Introduce_Complex_Generic_Types_609 {
    fn name(&self) -> &str {
        "Introduce_Complex_Generic_Types_609"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if !generics.params.is_empty() {
                    generics.params.push(parse_quote!(U: Clone + Default));
                    generics.params.push(parse_quote!('a));
                    if let Some(where_clause) = &mut generics.where_clause {
                        where_clause.predicates.push(parse_quote!(T: 'a));
                    } else {
                        generics.where_clause = Some(parse_quote!(where T: 'a));
                    }
                    if let Some(FnArg::Typed(pat_type)) = func.sig.inputs.first_mut() {
                        pat_type.ty = Box::new(parse_quote!(&'a U));
                    }
                }
            }
            if let syn::Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                if !generics.params.is_empty() {
                    generics.params.push(parse_quote!(U: PartialOrd + 'static));
                    if let Some(field) = item_struct.fields.iter_mut().next() {
                        if let syn::Type::Path(type_path) = &mut field.ty {
                            type_path.path.segments.push(parse_quote!(U));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}