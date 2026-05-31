use proc_macro2::{Span, TokenStream, TokenTree};
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

pub struct Convert_Inner_To_Outer_Attribute_480;

impl Mutator for Convert_Inner_To_Outer_Attribute_480 {
    fn name(&self) -> &str {
        "Convert_Inner_To_Outer_Attribute_480"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                let tokens = item_macro.mac.tokens.clone();
                let mut new_tokens = TokenStream::new();
                let mut iter = tokens.into_iter().peekable();
                while let Some(token) = iter.next() {
                    if let TokenTree::Punct(punct) = &token {
                        if punct.as_char() == '#' {
                            if let Some(TokenTree::Punct(next_punct)) = iter.peek() {
                                if next_punct.as_char() == '!' {
                                    iter.next(); // Consume '!'
                                    new_tokens.extend(quote! { # });
                                    continue;
                                }
                            }
                        }
                    }
                    new_tokens.extend(quote! { #token });
                }
                item_macro.mac.tokens = new_tokens;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies inner attributes within procedural macro outputs, indicated by the `#![...]` syntax. It transforms these inner attributes into outer attributes by removing the `!`, resulting in `#[...]`. This mutation aims to test the compiler's handling of attribute contexts, potentially leading to unexpected behavior or ICEs due to the altered attribute scope."
    }
}