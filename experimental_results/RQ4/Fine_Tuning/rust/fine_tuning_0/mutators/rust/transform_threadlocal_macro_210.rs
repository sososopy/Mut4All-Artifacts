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

pub struct Transform_ThreadLocal_Macro_210;

impl Mutator for Transform_ThreadLocal_Macro_210 {
    fn name(&self) -> &str {
        "Transform_ThreadLocal_Macro_210"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if let Some(mac) = item_macro.mac.path.segments.last() {
                    if mac.ident == "thread_local" {
                        if let Some(tt) = item_macro.mac.tokens.clone().into_iter().next() {
                            if let TokenTree::Group(group) = tt {
                                let mut new_tokens = TokenStream::new();
                                for token in group.stream() {
                                    new_tokens.extend(Some(token.clone()));
                                    if let TokenTree::Ident(ident) = &token {
                                        if ident == "static" {
                                            if let Some(TokenTree::Ident(var_ident)) = group.stream().into_iter().nth(1) {
                                                let complex_init: TokenStream = parse_quote! {
                                                    {
                                                        let x = #var_ident::new();
                                                        if condition() {
                                                            x.modify();
                                                        }
                                                        x
                                                    }
                                                };
                                                new_tokens.extend(Some(TokenTree::Group(Group::new(Delimiter::Brace, complex_init))));
                                            }
                                        }
                                    }
                                }
                                item_macro.mac.tokens = TokenStream::from(TokenTree::Group(Group::new(Delimiter::Brace, new_tokens)));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}