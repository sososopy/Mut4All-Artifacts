use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprMacro, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Macro_With_Keyword_207;

impl Mutator for Replace_Macro_With_Keyword_207 {
    fn name(&self) -> &str {
        "Replace_Macro_With_Keyword_207"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                for attr in &mut item_const.attrs {
                    if attr.path().is_ident("path") {
                        if let Ok(meta) = attr.parse_args::<syn::Meta>() {
                            if let syn::Meta::NameValue(name_value) = meta {
                                if let syn::Expr::Macro(expr_macro) = *name_value.value {
                                    let new_macro = parse_quote!(keyword_macro!());
                                    *name_value.value = Box::new(new_macro);
                                }
                            }
                        }
                    }
                }
            } else if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    for attr in &mut item_fn.attrs {
                        if attr.path().is_ident("path") {
                            if let Ok(meta) = attr.parse_args::<syn::Meta>() {
                                if let syn::Meta::NameValue(name_value) = meta {
                                    if let syn::Expr::Macro(expr_macro) = *name_value.value {
                                        let new_macro = parse_quote!(keyword_macro!());
                                        *name_value.value = Box::new(new_macro);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations within path attributes of const items and const functions. It replaces the macro invocation with a call to a macro (keyword_macro) that expands to a keyword (e.g., 'fn'), causing identifier conflicts and resolver errors. This stresses the compiler's macro expansion and resolver systems, potentially triggering ICEs or type-checking failures due to keyword misuse in identifier contexts."
    }
}