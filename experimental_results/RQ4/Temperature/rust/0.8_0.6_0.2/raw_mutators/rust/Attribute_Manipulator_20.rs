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

pub struct Attribute_Manipulator_20;

impl Mutator for Attribute_Manipulator_20 {
    fn name(&self) -> &str {
        "Attribute_Manipulator_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut rng = thread_rng();
                let attributes = &mut func.attrs;

                // Decide randomly to add, remove, or change an attribute
                let choice = rng.gen_range(0..3);

                match choice {
                    0 => { // Add an attribute
                        let new_attr: syn::Attribute = parse_quote!(#[inline]);
                        attributes.push(new_attr);
                    }
                    1 => { // Remove an attribute if it exists
                        if !attributes.is_empty() {
                            let index = rng.gen_range(0..attributes.len());
                            attributes.remove(index);
                        }
                    }
                    2 => { // Change an attribute if it exists
                        if !attributes.is_empty() {
                            let index = rng.gen_range(0..attributes.len());
                            attributes[index] = parse_quote!(#[deprecated]);
                        }
                    }
                    _ => {}
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with attributes and randomly chooses to add, remove, or change an attribute. This manipulation aims to explore how different attributes affect compiler diagnostics and behavior, potentially uncovering issues with attribute handling or linting processes."
    }
}