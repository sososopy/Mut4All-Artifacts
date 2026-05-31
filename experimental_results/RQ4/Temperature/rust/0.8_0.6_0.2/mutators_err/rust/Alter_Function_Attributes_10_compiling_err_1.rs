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

pub struct Alter_Function_Attributes_10;

impl Mutator for Alter_Function_Attributes_10 {
    fn name(&self) -> &str {
        "Alter_Function_Attributes_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_no_mangle = false;
                let mut new_attrs = vec![];

                for attr in &func.attrs {
                    if attr.path.is_ident("no_mangle") {
                        has_no_mangle = true;
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }

                if has_no_mangle {
                    // Randomly choose to add, remove, or replace the attribute
                    let mut rng = thread_rng();
                    match rng.gen_range(0..3) {
                        0 => { // Add an attribute
                            new_attrs.push(parse_quote!(#[inline(always)]));
                        }
                        1 => { // Remove the attribute
                            // Do nothing as #[no_mangle] is already omitted
                        }
                        2 => { // Replace the attribute
                            new_attrs.push(parse_quote!(#[inline]));
                        }
                        _ => {}
                    }
                }

                func.attrs = new_attrs;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with the #[no_mangle] attribute. It randomly decides to add a new #[inline(always)] attribute, remove the #[no_mangle] attribute, or replace it with #[inline]. This tests the Rust compiler's handling of function attributes, especially in scenarios involving FFI or specific runtime behaviors."
    }
}