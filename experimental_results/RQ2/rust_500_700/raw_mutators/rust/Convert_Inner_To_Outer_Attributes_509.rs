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

pub struct Convert_Inner_To_Outer_Attributes_509;

impl Mutator for Convert_Inner_To_Outer_Attributes_509 {
    fn name(&self) -> &str {
        "Convert_Inner_To_Outer_Attributes_509"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                if let Some((_, ref mut tokens)) = item_macro.mac.tokens.clone().into_iter().next() {
                    let mut new_tokens = proc_macro2::TokenStream::new();
                    let mut inner_attr_found = false;
                    for token in tokens.clone() {
                        if let proc_macro2::TokenTree::Group(group) = &token {
                            if group.delimiter() == proc_macro2::Delimiter::Brace {
                                let mut inner_tokens = proc_macro2::TokenStream::new();
                                for inner_token in group.stream() {
                                    if let proc_macro2::TokenTree::Punct(punct) = &inner_token {
                                        if punct.as_char() == '!' {
                                            inner_attr_found = true;
                                            continue;
                                        }
                                    }
                                    if inner_attr_found {
                                        if let proc_macro2::TokenTree::Ident(ident) = &inner_token {
                                            new_tokens.extend(quote! { #[#ident] });
                                            inner_attr_found = false;
                                            continue;
                                        }
                                    }
                                    inner_tokens.extend(quote! { #inner_token });
                                }
                                new_tokens.extend(quote! { { #inner_tokens } });
                            } else {
                                new_tokens.extend(quote! { #token });
                            }
                        } else {
                            new_tokens.extend(quote! { #token });
                        }
                    }
                    *tokens = new_tokens;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}