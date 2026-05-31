use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, AttrStyle, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, 
    GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, 
    MetaNameValue, Pat, PatType, Path as SynPath, PathArguments, 
    ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait, TypeParamBound, 
    TypePath, parse_quote, punctuated::Punctuated, spanned::Spanned, token, token::Comma, 
    token::{Paren, Plus}, visit::Visit, visit_mut::VisitMut, *};

use crate::mutator::Mutator;

pub struct Replace_Format_String_252;

impl Mutator for Replace_Format_String_252 {
    fn name(&self) -> &str {
        "Replace_Format_String_252"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for attr in &mut trait_item.attrs {
                    if let Meta::NameValue(MetaNameValue {
                        path,
                        value,
                        ..
                    }) = &attr.meta {
                        if path.is_ident("error") {
                            if let Expr::Lit(lit) = value {
                                if let Lit::Str(lit_str) = &lit.lit {
                                    let mut rng = thread_rng();
                                    let format_specifiers = ["{}", "{:?}", "{:#?}", "{:x}", "{:o}", "{:b}"];
                                    let chosen_specifier = *format_specifiers.choose(&mut rng).unwrap();
                                    let new_format_string = format!("{} {}", lit_str.value(), chosen_specifier);
                                    attr.meta = Meta::NameValue(MetaNameValue {
                                        path: path.clone(),
                                        eq_token: token::Eq::default(),
                                        value: Expr::Lit(Lit::Str(LitStr::new(&new_format_string, lit_str.span()))),
                                    });
                                }
                            }
                        }
                    }
                }
            } else if let Item::Impl(impl_item) = item {
                for attr in &mut impl_item.attrs {
                    if let Meta::NameValue(MetaNameValue {
                        path,
                        value,
                        ..
                    }) = &attr.meta {
                        if path.is_ident("error") {
                            if let Expr::Lit(lit) = value {
                                if let Lit::Str(lit_str) = &lit.lit {
                                    let mut rng = thread_rng();
                                    let format_specifiers = ["{}", "{:?}", "{:#?}", "{:x}", "{:o}", "{:b}"];
                                    let chosen_specifier = *format_specifiers.choose(&mut rng).unwrap();
                                    let new_format_string = format!("{} {}", lit_str.value(), chosen_specifier);
                                    attr.meta = Meta::NameValue(MetaNameValue {
                                        path: path.clone(),
                                        eq_token: token::Eq::default(),
                                        value: Expr::Lit(Lit::Str(LitStr::new(&new_format_string, lit_str.span()))),
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the format string in the error attribute of a derive macro, replacing it with a new string that may contain invalid format specifiers. This transformation aims to test the compiler's handling of invalid format strings in proc-macros and expose potential bugs in the proc-macro expansion process."
    }
}