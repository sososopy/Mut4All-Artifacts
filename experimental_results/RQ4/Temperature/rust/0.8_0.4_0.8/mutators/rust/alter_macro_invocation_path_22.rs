use proc_macro2::{Span, TokenStream, TokenTree, Group};
use quote::ToTokens;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, FnArg, Ident, Item, ItemFn, Path as SynPath, PathArguments, ReturnType, Stmt, 
    Type, parse_quote, punctuated::Punctuated, visit_mut::VisitMut, spanned::Spanned, token,
    token::{Comma, Paren, Plus}, visit::Visit, *,
};

use crate::mutator::Mutator;

pub struct Alter_Macro_Invocation_Path_22;

impl Mutator for Alter_Macro_Invocation_Path_22 {
    fn name(&self) -> &str {
        "Alter_Macro_Invocation_Path_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut alternative_path = None;

        for item in &mut file.items {
            if let Item::Mod(module) = item {
                if let Some((_, items)) = &module.content {
                    for item in items {
                        if let Item::Fn(func) = item {
                            if let Visibility::Public(_) = func.vis {
                                alternative_path = Some(SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segments = Punctuated::new();
                                        segments.push(PathSegment {
                                            ident: module.ident.clone(),
                                            arguments: PathArguments::None,
                                        });
                                        segments.push(PathSegment {
                                            ident: func.sig.ident.clone(),
                                            arguments: PathArguments::None,
                                        });
                                        segments
                                    },
                                });
                            }
                        }
                    }
                }
            }
        }

        if alternative_path.is_none() {
            let new_module: Item = parse_quote! {
                mod alternative_module {
                    pub fn some_function() {}
                }
            };
            file.items.push(new_module);

            alternative_path = Some(SynPath {
                leading_colon: None,
                segments: {
                    let mut segments = Punctuated::new();
                    segments.push(PathSegment {
                        ident: Ident::new("alternative_module", Span::call_site()),
                        arguments: PathArguments::None,
                    });
                    segments.push(PathSegment {
                        ident: Ident::new("some_function", Span::call_site()),
                        arguments: PathArguments::None,
                    });
                    segments
                },
            });
        }

        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                let tokens = &mut item_macro.mac.tokens;
                if let Some(TokenTree::Group(group)) = tokens.clone().into_iter().next() {
                    if let Some(TokenTree::Ident(ident)) = group.stream().into_iter().next() {
                        if ident.to_string().ends_with("::some_module") {
                            let new_tokens = TokenStream::from_iter([
                                TokenTree::Group(Group::new(
                                    group.delimiter(),
                                    alternative_path.clone().unwrap().to_token_stream(),
                                ))
                            ]);
                            *tokens = new_tokens;
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