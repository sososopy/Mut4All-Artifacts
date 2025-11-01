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

pub struct Inline_Const_Expr_In_Global_Asm_497;

impl Mutator for Inline_Const_Expr_In_Global_Asm_497 {
    fn name(&self) -> &str {
        "Inline_Const_Expr_In_Global_Asm_497"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if mac.mac.path.is_ident("global_asm") {
                    let mut tokens = mac.mac.tokens.clone().into_iter().collect::<Vec<_>>();
                    for token in &mut tokens {
                        if let TokenTree::Group(group) = token {
                            let mut new_stream = TokenStream::new();
                            for token in group.stream() {
                                if let TokenTree::Ident(ident) = &token {
                                    if ident == "const" {
                                        new_stream.extend(Some(token.clone()));
                                    }
                                } else if let TokenTree::Literal(lit) = &token {
                                    let lit_str = lit.to_string();
                                    if lit_str.contains('/') {
                                        let parts: Vec<&str> = lit_str.split('/').collect();
                                        if parts.len() == 2 && parts[1].trim() == "0" {
                                            let new_lit = format!("{}/1", parts[0].trim());
                                            new_stream.extend(Some(TokenTree::Literal(Literal::string(&new_lit))));
                                            continue;
                                        }
                                    }
                                    new_stream.extend(Some(token.clone()));
                                } else {
                                    new_stream.extend(Some(token.clone()));
                                }
                            }
                            *token = TokenTree::Group(Group::new(group.delimiter(), new_stream));
                        }
                    }
                    mac.mac.tokens = tokens.into_iter().collect();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `global_asm!` macros that use inline constant expressions. It specifically looks for division by zero within these expressions and replaces the divisor with a non-zero constant (1), preventing runtime errors. This mutation tests the handling of inline constant expressions in `global_asm!` macros without causing immediate runtime failures."
    }
}