use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Modify_Macro_Invocation_206;

impl Mutator for Modify_Macro_Invocation_206 {
    fn name(&self) -> &str {
        "Modify_Macro_Invocation_206"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(mac) = item {
                let mut new_stream = TokenStream::new();
                for token in mac.mac.clone().tokens {
                    match token {
                        TokenTree::Ident(ident) => {
                            if ident == "m" {
                                new_stream.extend(quote! { n });
                            } else {
                                new_stream.extend(quote! { #ident });
                            }
                        }
                        TokenTree::Punct(punct) => {
                            if punct.as_char() == '!' {
                                new_stream.extend(quote! { ? });
                            } else {
                                new_stream.extend(quote! { #punct });
                            }
                        }
                        TokenTree::Group(ref group) => {
                            if group.delimiter() == Delimiter::Parenthesis {
                                let mut new_group = Group::new(
                                    group.delimiter(),
                                    TokenStream::new(),
                                );
                                for token in group.stream() {
                                    match token {
                                        TokenTree::Ident(ident) => {
                                            if ident == "inline" {
                                                new_group.stream().extend(quote! { no_inline });
                                            } else if ident == "no_inline" {
                                                new_group.stream().extend(quote! { inline });
                                            } else {
                                                new_group.stream().extend(quote! { #ident });
                                            }
                                        }
                                        TokenTree::Punct(punct) => {
                                            new_group.stream().extend(quote! { #punct });
                                        }
                                        TokenTree::Group(inner_group) => {
                                            new_group.stream().extend(quote! { #inner_group });
                                        }
                                        _ => {
                                            new_group.stream().extend(quote! { #token });
                                        }
                                    }
                                }
                                new_stream.extend(quote! { #new_group });
                            } else {
                                new_stream.extend(quote! { #token });
                            }
                        }
                        _ => {
                            new_stream.extend(quote! { #token });
                        }
                    }
                }
                mac.mac.tokens = new_stream;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations in the given Rust code and modifies them by changing the arguments or the path to the macro. It applies to all macro invocations in the code. This transformation tests how the compiler handles different macro invocations and arguments, which could help expose bugs related to macro expansion and resolution."
    }
}

fn main() {
    let mut file_ast = parse_quote! {
        // your code here
    };
    Modify_Macro_Invocation_206.mutate(&mut file_ast);
}