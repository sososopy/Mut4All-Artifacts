use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemMacro, ItemStruct, Lifetime, LifetimeParam, Local, Macro, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote, punctuated::Punctuated, spanned::Spanned,
    token, token::Comma, token::{Paren, Plus}, visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Nested_Macros_With_Mismatched_Parameters_551;

impl Mutator for Nested_Macros_With_Mismatched_Parameters_551 {
    fn name(&self) -> &str {
        "Nested_Macros_With_Mismatched_Parameters_551"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if let Some((_, nested_macro)) = find_nested_macro(&item_macro.mac) {
                    let new_macro = introduce_parameter_mismatch(nested_macro);
                    item_macro.mac.tokens = new_macro.tokens;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn find_nested_macro(macro_: &Macro) -> Option<(Ident, Macro)> {
    let mut nested_macro = None;
    for token_tree in macro_.tokens.clone() {
        if let TokenTree::Group(group) = token_tree {
            for nested_token in group.stream() {
                if let TokenTree::Ident(ident) = nested_token {
                    nested_macro = Some((ident.clone(), macro_.clone()));
                    break;
                }
            }
        }
    }
    nested_macro
}

fn introduce_parameter_mismatch(mut nested_macro: Macro) -> Macro {
    let mut new_tokens = TokenStream::new();
    for token_tree in nested_macro.tokens {
        if let TokenTree::Ident(ident) = token_tree {
            if ident == "U" {
                new_tokens.extend(quote! { T });
            } else {
                new_tokens.extend(quote! { #ident });
            }
        } else {
            new_tokens.extend(quote! { #token_tree });
        }
    }
    nested_macro.tokens = new_tokens;
    nested_macro
}