use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Mutate_Inline_Assembly_Include_Strings_171;

impl Mutator for Mutate_Inline_Assembly_Include_Strings_171 {
    fn name(&self) -> &str {
        "Mutate_Inline_Assembly_Include_Strings_171"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(macro_item) = item {
                if macro_item.mac.path.is_ident("global_asm") || macro_item.mac.path.is_ident("asm") {
                    let mut tokens = macro_item.mac.tokens.clone();
                    let mut new_tokens = proc_macro2::TokenStream::new();
                    let mut iter = tokens.into_iter();
                    while let Some(token) = iter.next() {
                        if let proc_macro2::TokenTree::Literal(lit) = token {
                            let lit_str = lit.to_string();
                            if lit_str.starts_with('\"') && lit_str.contains("include_str") {
                                let new_lit = proc_macro2::Literal::string("include_str!(\"nonexistent.s\")");
                                new_tokens.extend(proc_macro2::TokenTree::Literal(new_lit));
                            } else {
                                new_tokens.extend(proc_macro2::TokenTree::Literal(lit));
                            }
                        } else if let proc_macro2::TokenTree::Group(group) = token {
                            let delimiter = group.delimiter();
                            let stream = group.stream();
                            let mut new_stream = proc_macro2::TokenStream::new();
                            let mut inner_iter = stream.into_iter();
                            while let Some(inner_token) = inner_iter.next() {
                                if let proc_macro2::TokenTree::Literal(inner_lit) = inner_token {
                                    let inner_lit_str = inner_lit.to_string();
                                    if inner_lit_str.starts_with('\"') && inner_lit_str.contains("include_str") {
                                        let new_inner_lit = proc_macro2::Literal::string("include_str!(\"invalid_assembly.s\")");
                                        new_stream.extend(proc_macro2::TokenTree::Literal(new_inner_lit));
                                    } else {
                                        new_stream.extend(proc_macro2::TokenTree::Literal(inner_lit));
                                    }
                                } else {
                                    new_stream.extend(proc_macro2::TokenTree::from(inner_token));
                                }
                            }
                            let new_group = proc_macro2::Group::new(delimiter, new_stream);
                            new_tokens.extend(proc_macro2::TokenTree::Group(new_group));
                        } else {
                            new_tokens.extend(proc_macro2::TokenTree::from(token));
                        }
                    }
                    macro_item.mac.tokens = new_tokens;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `global_asm!` and `asm!` macros with string literal arguments, especially those using `include_str!`. It modifies the assembly content by altering the included file path to a non-existent or invalid file, causing inclusion failures. Alternatively, it can inject synthetic assembly errors like unrecognized instructions or symbol conflicts. This aims to trigger assertion failures in the compiler's span handling during assembly parsing and diagnostic emission."
    }
}