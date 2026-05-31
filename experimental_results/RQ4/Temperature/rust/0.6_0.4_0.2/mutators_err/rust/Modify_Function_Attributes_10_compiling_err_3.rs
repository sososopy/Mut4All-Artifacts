use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Modify_Function_Attributes_10;

impl Mutator for Modify_Function_Attributes_10 {
    fn name(&self) -> &str {
        "Modify_Function_Attributes_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut rng = thread_rng();
                let remove_attribute = rng.gen_bool(0.5);

                item_fn.attrs.iter_mut().for_each(|attr| {
                    if attr.path().is_ident("no_mangle") {
                        if remove_attribute {
                            attr.tokens.clear();
                        } else {
                            let new_attr: syn::Attribute = parse_quote!(#[inline(always)]);
                            *attr = new_attr;
                        }
                    }
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with the `#[no_mangle]` attribute, either replacing it with `#[inline(always)]` or removing it entirely. This transformation tests the compiler's handling of function attributes, particularly in scenarios where naming and linkage are critical, potentially exposing issues with function attribute processing in complex codebases."
    }
}