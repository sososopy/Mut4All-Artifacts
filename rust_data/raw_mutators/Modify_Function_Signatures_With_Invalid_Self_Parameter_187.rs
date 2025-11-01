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

pub struct Modify_Function_Signatures_With_Invalid_Self_Parameter_187;

impl Mutator for Modify_Function_Signatures_With_Invalid_Self_Parameter_187 {
    fn name(&self) -> &str {
        "Modify_Function_Signatures_With_Invalid_Self_Parameter_187"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !is_in_impl_or_trait(item_fn) {
                    let self_arg: FnArg = parse_quote!(&self);
                    item_fn.sig.inputs.insert(0, self_arg);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies function signatures by adding an invalid `&self` parameter to functions that are not within `impl` blocks or trait definitions. This creates a semantic error, as `&self` is only valid in associated functions. The mutation isolates the change to the function signature, potentially triggering compiler errors related to incorrect usage of `self`."
    }
}

fn is_in_impl_or_trait(item_fn: &ItemFn) -> bool {
    let mut parent = item_fn.attrs.iter().find_map(|attr| {
        if let Some(segment) = attr.path.segments.first() {
            if segment.ident == "impl" || segment.ident == "trait" {
                return Some(attr);
            }
        }
        None
    });
    parent.is_some()
}