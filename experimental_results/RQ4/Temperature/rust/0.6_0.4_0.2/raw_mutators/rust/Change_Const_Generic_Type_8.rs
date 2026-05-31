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

pub struct Change_Const_Generic_Type_8;

impl Mutator for Change_Const_Generic_Type_8 {
    fn name(&self) -> &str {
        "Change_Const_Generic_Type_8"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, generics, _)) = &item_impl.generics.split_for_impl() {
                    for param in &mut generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(TypePath { path, .. }) = &*const_param.ty {
                                if let Some(segment) = path.segments.last() {
                                    if segment.ident == "u64" {
                                        const_param.ty = Box::new(parse_quote!(usize));
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
        "The mutation operator targets const generics in trait implementations, specifically changing their type from `u64` to `usize`. This transformation tests the compiler's ability to handle const generics of different types and may reveal issues in type inference or constant evaluation."
    }
}