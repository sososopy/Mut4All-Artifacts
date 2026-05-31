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

pub struct Modify_Function_Annotations_20;

impl Mutator for Modify_Function_Annotations_20 {
    fn name(&self) -> &str {
        "Modify_Function_Annotations_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                func.attrs.retain(|attr| {
                    !attr.path().is_ident("must_use")
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with the #[must_use] annotation, removing it to observe changes in compiler behavior. By altering these annotations, we can explore potential issues in lint processing or diagnostics related to function return values."
    }
}