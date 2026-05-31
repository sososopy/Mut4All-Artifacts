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
    Meta, MetaList,
};

use crate::mutator::Mutator;

pub struct Inline_Attribute_Removal_344;

impl Mutator for Inline_Attribute_Removal_344 {
    fn name(&self) -> &str {
        "Inline_Attribute_Removal_344"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                func.attrs.retain(|attr| {
                    let mut should_retain = true;
                    if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                        if meta_list.path.is_ident("inline") {
                            meta_list.tokens.to_string().split(',').for_each(|nested_meta| {
                                if nested_meta.trim() == "always" {
                                    should_retain = false;
                                }
                            });
                        }
                    }
                    should_retain
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with the `#[inline(always)]` attribute, removing the attribute to test how the compiler handles the absence of forced inlining. This aims to explore potential issues in optimization and code generation when the inlining directive is altered, potentially uncovering bugs in the compiler's inlining logic."
    }
}