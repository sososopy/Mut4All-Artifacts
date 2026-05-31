use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Remove_Must_Use_Attribute_197;

impl Mutator for Remove_Must_Use_Attribute_197 {
    fn name(&self) -> &str {
        "Remove_Must_Use_Attribute_197"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                func.attrs.retain(|attr| {
                    if let Ok(met) = attr.parse_meta().map_err(|_| ()) {
                        if met.path().is_ident("must_use") {
                            return false;
                        }
                    }
                    true
                });
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes the `must_use` attribute from functions, testing the compiler's handling of return value usage and potential bugs related to the attribute's interaction with function calls and return values."
    }
}