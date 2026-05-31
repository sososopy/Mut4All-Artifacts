use proc_macro2::*;
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Modify_Const_Generics_Type_8;

impl Mutator for Modify_Const_Generics_Type_8 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_Type_8"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for param in &mut item_impl.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.ty == parse_quote!(usize) {
                            const_param.ty = parse_quote!(u64);
                        }
                    }
                }
            }
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.ty == parse_quote!(usize) {
                            const_param.ty = parse_quote!(u64);
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