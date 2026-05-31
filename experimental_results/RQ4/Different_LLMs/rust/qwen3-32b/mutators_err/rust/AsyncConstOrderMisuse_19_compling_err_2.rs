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
use syn::token::Async;

use crate::mutator::Mutator;

pub struct AsyncConstOrderMisuse_19;

impl Mutator for AsyncConstOrderMisuse_19 {
    fn name(&self) -> &str {
        "AsyncConstOrderMisuse_19"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if impl_item.trait_.is_some() {
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(func) = impl_item {
                            if func.sig.constness.is_none() && func.sig.asyncness.is_none() {
                                func.sig.constness = Some(token::Const {
                                    span: Span::call_site(),
                                });
                                func.sig.asyncness = Some(Async {
                                    async_token: token::Async {
                                        span: Span::call_site(),
                                    },
                                    generic_params: None,
                                });
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