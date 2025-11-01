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

pub struct Modify_Const_Function_In_Trait_Impl_128;

impl Mutator for Modify_Const_Function_In_Trait_Impl_128 {
    fn name(&self) -> &str {
        "Modify_Const_Function_In_Trait_Impl_128"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Method(method) = impl_item {
                        if method.sig.constness.is_some() {
                            method.sig.constness = None;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets const functions within trait implementations. It identifies const functions and removes the `const` keyword, converting them into non-const functions while preserving the original functionality."
    }
}