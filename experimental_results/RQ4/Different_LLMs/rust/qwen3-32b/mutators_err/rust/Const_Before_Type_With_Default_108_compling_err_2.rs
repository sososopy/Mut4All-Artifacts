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

pub struct Const_Before_Type_With_Default_108;

impl Mutator for Const_Before_Type_With_Default_108 {
    fn name(&self) -> &str {
        "Const_Before_Type_With_Default_108"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                process_generics(&mut s.generics.params);
            } else if let syn::Item::Enum(e) = item {
                process_generics(&mut e.generics.params);
            } else if let syn::Item::Trait(t) = item {
                process_generics(&mut t.generics.params);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn process_generics(params: &mut Punctuated<GenericParam, Comma>) {
    let has_const = params.iter().any(|p| matches!(p, GenericParam::Const(_)));
    let has_type = params.iter().any(|p| matches!(p, GenericParam::Type(_)));
    if !(has_const && has_type) {
        return;
    }

    if let Some(idx) = params.iter().position(|p| matches!(p, GenericParam::Const(_))) {
        let const_param = params.remove(idx).unwrap();
        if let GenericParam::Const(mut cp) = const_param {
            if cp.default.is_none() {
                cp.default = Some(parse_quote!({0}));
            }
            params.insert(0, GenericParam::Const(cp));
        }
    }
}