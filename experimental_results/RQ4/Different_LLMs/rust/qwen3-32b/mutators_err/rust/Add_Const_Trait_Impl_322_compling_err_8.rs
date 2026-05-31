use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};
use syn::visit::Defaultness;

use crate::mutator::Mutator;

pub struct Add_Const_Trait_Impl_322;

impl Mutator for Add_Const_Trait_Impl_322 {
    fn name(&self) -> &str {
        "Add_Const_Trait_Impl_322"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if item_impl.defaultness.is_none() {
                    item_impl.defaultness = Some(Defaultness::Const(token::Const {
                        span: proc_macro2::Span::call_site(),
                    }));
                } else if let Some(Defaultness::Default(_)) = item_impl.defaultness {
                    item_impl.defaultness = Some(Defaultness::Const(token::Const {
                        span: proc_macro2::Span::call_site(),
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}