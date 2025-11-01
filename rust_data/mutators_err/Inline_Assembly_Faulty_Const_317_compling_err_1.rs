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

pub struct Inline_Assembly_Faulty_Const_317;

impl Mutator for Inline_Assembly_Faulty_Const_317 {
    fn name(&self) -> &str {
        "Inline_Assembly_Faulty_Const_317"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(ref mut macro_item) = item {
                if macro_item.mac.path.is_ident("global_asm") || macro_item.mac.path.is_ident("asm") {
                    let mut tokens = macro_item.mac.tokens.clone().into_iter().collect::<Vec<_>>();
                    for token in &mut tokens {
                        if let TokenTree::Group(group) = token {
                            let mut new_stream = TokenStream::new();
                            for inner_token in group.stream() {
                                if let TokenTree::Ident(ident) = &inner_token {
                                    if ident == "const" {
                                        new_stream.extend(quote! { const 1 / 0 });
                                    } else {
                                        new_stream.extend(quote! { #inner_token });
                                    }
                                } else {
                                    new_stream.extend(quote! { #inner_token });
                                }
                            }
                            *token = TokenTree::Group(Group::new(group.delimiter(), new_stream));
                        }
                    }
                    macro_item.mac.tokens = TokenStream::from_iter(tokens);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly expressions in Rust code, specifically modifying constant expressions used in `global_asm!` or `asm!` macros. The operator identifies an existing macro invocation, selects a constant expression, and modifies it to introduce an undefined or erroneous behavior, such as division by zero. This mutation is likely to trigger an internal compiler error due to the unexpected constant evaluation within the context of inline assembly."
    }
}