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

pub struct Add_No_Mangle_To_Functions_10;

impl Mutator for Add_No_Mangle_To_Functions_10 {
    fn name(&self) -> &str {
        "Add_No_Mangle_To_Functions_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let has_no_mangle = func.attrs.iter().any(|attr| {
                    if let Ok(meta) = attr.parse_meta() {
                        if let syn::Meta::Path(path) = meta {
                            return path.is_ident("no_mangle");
                        }
                    }
                    false
                });

                if !has_no_mangle {
                    func.attrs.push(parse_quote!(#[no_mangle]));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds the `#[no_mangle]` attribute to all function definitions that do not already have it. This transformation exposes potential issues with symbol mangling and linkage, testing the compiler's ability to handle symbol visibility and linkage changes."
    }
}