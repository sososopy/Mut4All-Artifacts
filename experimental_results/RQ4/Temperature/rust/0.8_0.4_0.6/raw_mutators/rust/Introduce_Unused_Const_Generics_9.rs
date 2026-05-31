use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Introduce_Unused_Const_Generics_9;

impl Mutator for Introduce_Unused_Const_Generics_9 {
    fn name(&self) -> &str {
        "Introduce_Unused_Const_Generics_9"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(func) => {
                    if func.sig.generics.params.iter().any(|param| matches!(param, GenericParam::Const(_))) {
                        func.sig.generics.params.push(parse_quote!(const UNUSED: usize));
                    }
                }
                Item::Struct(item_struct) => {
                    if item_struct.generics.params.iter().any(|param| matches!(param, GenericParam::Const(_))) {
                        item_struct.generics.params.push(parse_quote!(const UNUSED: usize));
                    }
                }
                Item::Trait(item_trait) => {
                    if item_trait.generics.params.iter().any(|param| matches!(param, GenericParam::Const(_))) {
                        item_trait.generics.params.push(parse_quote!(const UNUSED: usize));
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}