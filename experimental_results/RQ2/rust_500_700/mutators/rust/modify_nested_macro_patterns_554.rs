use proc_macro2::{Group, Punct, TokenStream, TokenTree};
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

pub struct Modify_Nested_Macro_Patterns_554;

impl Mutator for Modify_Nested_Macro_Patterns_554 {
    fn name(&self) -> &str {
        "Modify_Nested_Macro_Patterns_554"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some((_, nested_mac)) = mac.mac.tokens.clone().into_iter().enumerate().find_map(|(i, tt)| {
                    if let TokenTree::Group(group) = tt {
                        if let Some(nested_mac) = group.stream().into_iter().find_map(|tt| {
                            if let TokenTree::Ident(ident) = tt {
                                if ident == "macro_rules" {
                                    return Some(group.clone());
                                }
                            }
                            None
                        }) {
                            return Some((i, nested_mac));
                        }
                    }
                    None
                }) {
                    let mut new_stream = TokenStream::new();
                    for tt in nested_mac.stream() {
                        if let TokenTree::Group(group) = tt {
                            new_stream.extend(Some(TokenTree::Group(Group::new(
                                group.delimiter(),
                                group.stream().into_iter().map(|tt| {
                                    if let TokenTree::Group(inner_group) = tt {
                                        let mut mutated_stream = TokenStream::new();
                                        for inner_tt in inner_group.stream() {
                                            if let TokenTree::Punct(punct) = &inner_tt {
                                                if punct.as_char() == '*' {
                                                    mutated_stream.extend(Some(TokenTree::Punct(
                                                        Punct::new('+', punct.spacing()),
                                                    )));
                                                } else {
                                                    mutated_stream.extend(Some(inner_tt));
                                                }
                                            } else {
                                                mutated_stream.extend(Some(inner_tt));
                                            }
                                        }
                                        return TokenTree::Group(Group::new(
                                            inner_group.delimiter(),
                                            mutated_stream,
                                        ));
                                    }
                                    tt
                                }).collect(),
                            ))));
                        } else {
                            new_stream.extend(Some(tt));
                        }
                    }
                    if let Some(TokenTree::Group(group)) = mac.mac.tokens.clone().into_iter().find(|tt| {
                        if let TokenTree::Group(g) = tt {
                            g.delimiter() == nested_mac.delimiter() && g.stream().to_string() == nested_mac.stream().to_string()
                        } else {
                            false
                        }
                    }) {
                        mac.mac.tokens = new_stream;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}