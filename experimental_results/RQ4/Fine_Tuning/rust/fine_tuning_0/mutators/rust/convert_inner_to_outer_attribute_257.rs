use proc_macro2::{Span, TokenStream, TokenTree, Group, Delimiter};
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

pub struct Convert_Inner_To_Outer_Attribute_257;

impl Mutator for Convert_Inner_To_Outer_Attribute_257 {
    fn name(&self) -> &str {
        "Convert_Inner_To_Outer_Attribute_257"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                let tokens = item_macro.mac.tokens.clone();
                let mut new_tokens = TokenStream::new();
                for token in tokens {
                    if let TokenTree::Group(group) = token {
                        if group.delimiter() == Delimiter::Brace {
                            let mut inner_tokens = TokenStream::new();
                            for inner_token in group.stream() {
                                if let TokenTree::Punct(punct) = &inner_token {
                                    if punct.as_char() == '!' {
                                        continue;
                                    }
                                }
                                inner_tokens.extend(Some(inner_token));
                            }
                            new_tokens.extend(Some(TokenTree::Group(
                                Group::new(Delimiter::Brace, inner_tokens),
                            )));
                        } else {
                            new_tokens.extend(Some(TokenTree::Group(group)));
                        }
                    } else {
                        new_tokens.extend(Some(token));
                    }
                }
                item_macro.mac.tokens = new_tokens;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies inner attributes within procedural macro outputs and converts them to outer attributes. This transformation aims to test the compiler's handling of attribute contexts, potentially leading to ICEs or unexpected behavior in procedural macro expansions."
    }
}