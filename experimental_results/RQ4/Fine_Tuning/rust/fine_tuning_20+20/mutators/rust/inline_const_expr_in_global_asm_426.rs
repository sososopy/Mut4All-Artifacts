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

pub struct Inline_Const_Expr_In_Global_Asm_426;

impl Mutator for Inline_Const_Expr_In_Global_Asm_426 {
    fn name(&self) -> &str {
        "Inline_Const_Expr_In_Global_Asm_426"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_candidates: Vec<Ident> = Vec::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                const_candidates.push(item_const.ident.clone());
            }
        }
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if item_macro.mac.path.is_ident("global_asm") {
                    let mut tokens = item_macro.mac.tokens.clone().into_iter().peekable();
                    let mut new_tokens = proc_macro2::TokenStream::new();
                    while let Some(token) = tokens.next() {
                        if let proc_macro2::TokenTree::Ident(ident) = &token {
                            if ident == "const" {
                                new_tokens.extend(quote! { const });
                                if let Some(proc_macro2::TokenTree::Literal(literal)) =
                                    tokens.peek()
                                {
                                    let literal_str = literal.to_string();
                                    if literal_str.contains('/') {
                                        let parts: Vec<&str> = literal_str.split('/').collect();
                                        if parts.len() == 2 {
                                            let numerator = parts[0].trim();
                                            let denominator = parts[1].trim();
                                            if denominator == "0" {
                                                if !const_candidates.is_empty() {
                                                    let chosen_const = const_candidates
                                                        .choose(&mut thread_rng())
                                                        .unwrap();
                                                    new_tokens.extend(quote! { #numerator / #chosen_const });
                                                } else {
                                                    new_tokens.extend(quote! { #numerator / 1 });
                                                }
                                            } else {
                                                new_tokens.extend(quote! { #literal });
                                            }
                                        } else {
                                            new_tokens.extend(quote! { #literal });
                                        }
                                    } else {
                                        new_tokens.extend(quote! { #literal });
                                    }
                                }
                            } else {
                                new_tokens.extend(quote! { #ident });
                            }
                        } else {
                            new_tokens.extend(quote! { #token });
                        }
                    }
                    item_macro.mac.tokens = new_tokens;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets `global_asm!` macros with inline constant expressions, specifically addressing division by zero issues. It replaces zero divisors with existing constants or a default non-zero value, ensuring syntactic validity while testing the compiler's handling of inline const expressions in global assembly contexts."
    }
}