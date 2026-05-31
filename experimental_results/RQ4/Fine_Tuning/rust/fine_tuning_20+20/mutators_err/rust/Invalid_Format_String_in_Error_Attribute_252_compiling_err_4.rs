use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, str::FromStr, *};
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

pub struct Invalid_Format_String_in_Error_Attribute_252;

impl Mutator for Invalid_Format_String_in_Error_Attribute_252 {
    fn name(&self) -> &str {
        "Invalid_Format_String_in_Error_Attribute_252"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let has_error_derive = item_enum.attrs.iter().any(|attr| {
                    attr.path().is_ident("derive")
                        && attr
                            .parse_args_with(Punctuated::<SynPath, Token![,]>::parse_terminated)
                            .map(|paths| paths.iter().any(|path| path.is_ident("Error")))
                            .unwrap_or(false)
                });

                if has_error_derive {
                    for variant in &mut item_enum.variants {
                        for attr in &mut variant.attrs {
                            if attr.path().is_ident("error") {
                                let mut tokens = attr.meta.clone().into_token_stream().into_iter().peekable();
                                if let Some(proc_macro2::TokenTree::Group(group)) = tokens.next() {
                                    if group.delimiter() == proc_macro2::Delimiter::Parenthesis {
                                        let mut content = group.stream().to_string();
                                        if !content.contains('{') {
                                            content.push_str("{0.invalid}");
                                        } else {
                                            let mut replacements = Vec::new();
                                            for cap in Regex::new(r"\{([^}]+)\}")
                                                .unwrap()
                                                .captures_iter(&content)
                                            {
                                                let original = cap.get(0).unwrap().as_str();
                                                let replacement = format!("{{{}.invalid}}", &cap[1]);
                                                replacements.push((original, replacement));
                                            }
                                            for (original, replacement) in replacements {
                                                content = content.replace(original, &replacement);
                                            }
                                        }
                                        let new_group = proc_macro2::Group::new(
                                            proc_macro2::Delimiter::Parenthesis,
                                            proc_macro2::TokenStream::from_str(&content).unwrap(),
                                        );
                                        let mut new_tokens = proc_macro2::TokenStream::new();
                                        new_tokens.extend(Some(proc_macro2::TokenTree::Group(
                                            new_group,
                                        )));
                                        tokens = new_tokens.into_iter().peekable();
                                    }
                                }
                                attr.meta = syn::Meta::from(attr.path.clone());
                                attr.tokens = proc_macro2::TokenStream::from_iter(tokens);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets enums with `#[derive(Debug, Error)]` and modifies `#[error(\"...\")]` attributes by introducing invalid format specifiers like `{0.invalid}`. This transformation exploits the proc-macro system's reliance on format string correctness, aiming to trigger macro expansion errors and test the compiler's resilience to malformed macro input."
    }
}