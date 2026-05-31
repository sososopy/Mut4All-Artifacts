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

pub struct Alter_Lifetimes_In_Fn_Signatures_40;

impl Mutator for Alter_Lifetimes_In_Fn_Signatures_40 {
    fn name(&self) -> &str {
        "Alter_Lifetimes_In_Fn_Signatures_40"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(bound_lifetimes) = &mut func.sig.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        Some(lifetime_param)
                    } else {
                        None
                    }
                }) {
                    let new_lifetime_ident = Ident::new("b", Span::call_site());
                    let new_lifetime = Lifetime::new("'b", Span::call_site());
                    bound_lifetimes.bounds.push(new_lifetime.clone());

                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &mut *pat_type.ty {
                                if let Some(last_segment) = type_path.path.segments.last_mut() {
                                    if let PathArguments::AngleBracketed(arguments) = &mut last_segment.arguments {
                                        arguments.args.push(GenericArgument::Lifetime(new_lifetime));
                                    }
                                }
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