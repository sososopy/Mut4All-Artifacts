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

pub struct Add_Unused_Lifetimes_357;

impl Mutator for Add_Unused_Lifetimes_357 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetimes_357"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetimes = &mut func.sig.generics.params;
                lifetimes.push(parse_quote!('a));
                lifetimes.push(parse_quote!('b));
                lifetimes.push(parse_quote!('c));
            }
            if let syn::Item::Struct(item_struct) = item {
                let lifetimes = &mut item_struct.generics.params;
                lifetimes.push(parse_quote!('d));
                lifetimes.push(parse_quote!('e));
                lifetimes.push(parse_quote!('f));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused lifetime parameters to function and struct definitions. This can expose issues in the compiler's handling of lifetimes, particularly in scenarios involving lifetime resolution and variance. By introducing extraneous lifetimes, it aims to trigger ICEs or other bugs related to lifetime management."
    }
}