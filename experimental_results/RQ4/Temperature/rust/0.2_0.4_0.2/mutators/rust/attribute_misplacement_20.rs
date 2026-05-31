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

pub struct Attribute_Misplacement_20;

impl Mutator for Attribute_Misplacement_20 {
    fn name(&self) -> &str {
        "Attribute_Misplacement_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut functions_with_attrs = Vec::new();
        let mut functions_without_attrs = Vec::new();

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if !func.attrs.is_empty() {
                    functions_with_attrs.push(func);
                } else {
                    functions_without_attrs.push(func);
                }
            }
        }

        if let Some(func_with_attr) = functions_with_attrs.choose_mut(&mut rng) {
            if let Some(attr) = func_with_attr.attrs.pop() {
                if let Some(func_without_attr) = functions_without_attrs.choose_mut(&mut rng) {
                    func_without_attr.attrs.push(attr);
                } else {
                    // Create a new function and apply the attribute
                    let new_func: ItemFn = parse_quote! {
                        fn new_function() -> i32 {
                            42
                        }
                    };
                    let mut new_func = new_func.clone();
                    new_func.attrs.push(attr);
                    file.items.push(Item::Fn(new_func));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with attributes and repositions these attributes to other functions without attributes or newly created functions. This transformation tests the compiler's handling of attribute enforcement and exposes potential issues in attribute-related logic by creating semantically questionable attribute placements."
    }
}