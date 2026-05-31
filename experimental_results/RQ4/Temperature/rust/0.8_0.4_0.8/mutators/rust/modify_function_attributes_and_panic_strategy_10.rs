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

pub struct Modify_Function_Attributes_And_Panic_Strategy_10;

impl Mutator for Modify_Function_Attributes_And_Panic_Strategy_10 {
    fn name(&self) -> &str {
        "Modify_Function_Attributes_And_Panic_Strategy_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_no_mangle = false;
                func.attrs.retain(|attr| {
                    if attr.path().is_ident("no_mangle") {
                        has_no_mangle = true;
                        false
                    } else {
                        true
                    }
                });

                if has_no_mangle {
                    let mut rng = thread_rng();
                    let choice = rng.gen_range(0..3);
                    match choice {
                        0 => func.attrs.push(parse_quote!(#[inline(always)])),
                        1 => func.attrs.push(parse_quote!(#[inline(never)])),
                        _ => {
                            func.attrs.push(parse_quote!(#[cold]));
                            func.attrs.push(parse_quote!(#[no_mangle]));
                        }
                    }
                } else {
                    func.attrs.push(parse_quote!(#[no_mangle]));
                    func.attrs.push(parse_quote!(#[inline]));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with the #[no_mangle] attribute, altering their attributes to explore different compiler behaviors. It swaps #[no_mangle] with #[inline(always)], #[inline(never)], or adds #[cold] alongside #[no_mangle]. It also adds #[no_mangle] and #[inline] to functions not originally marked. This aims to test the interaction between function attributes and panic strategies, particularly under 'panic=abort' and 'panic=unwind' configurations."
    }
}