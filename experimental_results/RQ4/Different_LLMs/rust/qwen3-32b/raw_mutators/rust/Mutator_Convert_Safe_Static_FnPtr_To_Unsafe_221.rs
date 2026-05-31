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

pub struct Mutator_Convert_Safe_Static_FnPtr_To_Unsafe_221;

impl Mutator for Mutator_Convert_Safe_Static_FnPtr_To_Unsafe_221 {
    fn name(&self) -> &str {
        "Convert_Safe_Static_FnPtr_To_Unsafe_221"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                if let syn::Type::BareFn(bare_fn) = &mut static_item.ty {
                    if bare_fn.unsafety.is_none() {
                        if let Some(init) = &mut static_item.expr {
                            if let syn::Expr::Closure(_) = **init {
                                bare_fn.unsafety = Some(syn::token::Unsafe::default());
                            }
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