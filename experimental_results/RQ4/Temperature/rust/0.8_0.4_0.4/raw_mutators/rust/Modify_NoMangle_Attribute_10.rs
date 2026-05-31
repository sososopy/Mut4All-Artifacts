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

pub struct Modify_NoMangle_Attribute_10;

impl Mutator for Modify_NoMangle_Attribute_10 {
    fn name(&self) -> &str {
        "Modify_NoMangle_Attribute_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                func.attrs.retain(|attr| {
                    if attr.path.is_ident("no_mangle") {
                        false
                    } else {
                        true
                    }
                });

                if thread_rng().gen_bool(0.5) {
                    func.attrs.push(parse_quote!(#[inline]));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with the `#[no_mangle]` attribute, removing or replacing it with `#[inline]`. This tests the impact of changing function linkage visibility on the program, potentially exposing symbol visibility issues or compiler behavior changes."
    }
}