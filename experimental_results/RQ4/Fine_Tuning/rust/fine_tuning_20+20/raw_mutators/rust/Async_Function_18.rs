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

pub struct Async_Function_18;

impl Mutator for Async_Function_18 {
    fn name(&self) -> &str {
        "Async_Function_18"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, _) = item_fn.sig.output {
                    item_fn.sig.asyncness = Some(token::Async::default());
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets functions with a return type and transforms them into asynchronous functions by adding the `async` keyword to their signature. This mutation leverages Rust's async/await syntax to alter the function's execution model, potentially uncovering issues related to async function lowering, future generation, and runtime behavior."
    }
}