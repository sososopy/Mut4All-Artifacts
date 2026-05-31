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

pub struct Add_Async_To_Const_Generic_With_ImplTrait_Alias_488;

impl Mutator for Add_Async_To_Const_Generic_With_ImplTrait_Alias_488 {
    fn name(&self) -> &str {
        "Add_Async_To_Const_Generic_With_ImplTrait_Alias_488"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_none() {
                    let has_const_with_type_path = func.sig.generics.params.iter().any(|param| {
                        if let syn::GenericParam::Const(const_param) = param {
                            if let Some(ty) = &const_param.ty {
                                matches!(ty, syn::Type::Path(_))
                            } else {
                                false
                            }
                        } else {
                            false
                        }
                    });
                    if has_const_with_type_path {
                        func.sig.asyncness = Some(syn::Async::new(Span::call_site()));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}