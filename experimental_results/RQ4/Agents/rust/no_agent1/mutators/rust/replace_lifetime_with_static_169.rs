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

pub struct Replace_Lifetime_With_Static_169;

impl Mutator for Replace_Lifetime_With_Static_169 {
    fn name(&self) -> &str {
        "Replace_Lifetime_With_Static_169"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                for param in &mut generics.params {
                    if let syn::GenericParam::Lifetime(lifetime_param) = param {
                        lifetime_param.lifetime = syn::Lifetime::new("'static", Span::call_site());
                    }
                }
            }
            if let syn::Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                for param in &mut generics.params {
                    if let syn::GenericParam::Lifetime(lifetime_param) = param {
                        lifetime_param.lifetime = syn::Lifetime::new("'static", Span::call_site());
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces all lifetime parameters in function and struct definitions with `'static`. This transformation alters the expected lifetime constraints, potentially leading to lifetime resolution issues, borrow checker errors, or ICEs. It aims to test the compiler's handling of static lifetimes in various contexts."
    }
}