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

pub struct Literal_Extraction_Enhancement_513;

impl Mutator for Literal_Extraction_Enhancement_513 {
    fn name(&self) -> &str {
        "Literal_Extraction_Enhancement_513"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                let mut new_tokens = TokenStream::new();
                let mut tokens_iter = mac.mac.tokens.clone().into_iter();
                while let Some(token) = tokens_iter.next() {
                    if let TokenTree::Group(group) = &token {
                        let mut group_stream = TokenStream::new();
                        let mut group_iter = group.stream().into_iter();
                        while let Some(inner_token) = group_iter.next() {
                            if let TokenTree::Literal(literal) = &inner_token {
                                new_tokens.extend(quote!(() => { 42 }));
                                new_tokens.extend(quote!(() => { 3.14 }));
                                new_tokens.extend(quote!(() => { 'c' }));
                                new_tokens.extend(quote!(() => { true }));
                                new_tokens.extend(quote!(() => { 5 + 10 }));
                            }
                            group_stream.extend(Some(inner_token));
                        }
                        new_tokens.extend(Some(TokenTree::Group(Group::new(group.delimiter(), group_stream))));
                    } else {
                        new_tokens.extend(Some(token));
                    }
                }
                mac.mac.tokens = new_tokens;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}