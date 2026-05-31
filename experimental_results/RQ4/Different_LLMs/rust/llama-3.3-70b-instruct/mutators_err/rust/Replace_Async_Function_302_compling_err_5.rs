use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Async_Function_302;

impl Mutator for Replace_Async_Function_302 {
    fn name(&self) -> &str {
        "Replace_Async_Function_302"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = item {
                        if let Some(asyncness) = &method.sig.asyncness {
                            if let syn::Async::_ = asyncness {
                                method.sig.asyncness = None;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The Replace_Async_Function_302 mutation operator targets async functions defined in traits. It applies the mutation by changing the async function to a regular function, removing the async keyword from the function definition. This transformation is likely to trigger bugs related to the async_fn_in_trait feature, as the bug report suggests that the compiler is not handling async functions in traits correctly."
    }
}