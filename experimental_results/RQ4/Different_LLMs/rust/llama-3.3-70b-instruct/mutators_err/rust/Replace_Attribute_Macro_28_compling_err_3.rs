use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use rand::prelude::IteratorRandom;
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Attribute_Macro_28;

impl Mutator for Replace_Attribute_Macro_28 {
    fn name(&self) -> &str {
        "Replace_Attribute_Macro_28"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut attribute_macros: Vec<Expr> = Vec::new();
        for item in &file.items {
            if let Item::Fn(func) = item {
                for attribute in &func.attrs {
                    if let Attribute { style: _, path, pound_token: _, bracket_token: _, meta } = attribute {
                        if let Some(segment) = path.segments.last() {
                            if segment.ident.to_string().ends_with("!") {
                                attribute_macros.push(parse_quote!(#path));
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for attribute in &mut func.attrs {
                    if let Attribute { style: _, path, pound_token: _, bracket_token: _, meta } = attribute {
                        if let Some(segment) = path.segments.last() {
                            if segment.ident.to_string().ends_with("!") {
                                if let Some(new_macro) = attribute_macros.iter().choose(&mut thread_rng()) {
                                    *attribute = parse_quote!(#[#new_macro]);
                                } else {
                                    let new_macro_name = Ident::new(&format!("new_macro_{}", thread_rng().gen::<u32>()), Span::call_site());
                                    let new_macro = parse_quote!(#new_macro_name!());
                                    *attribute = parse_quote!(#[#new_macro]);
                                    file.items.push(parse_quote!(macro_rules! #new_macro_name { ($($tt:tt)*) => { }; }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces attribute macro invocations with new ones that have a similar structure but with different parameters, or with different attribute macro invocations that are already present in the seed program. This transformation aims to test the compiler's ability to handle different attribute macro invocations and to expose bugs related to attribute parsing."
    }
}