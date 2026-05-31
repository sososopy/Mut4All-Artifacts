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

pub struct Inline_Asm_Faulty_Const_175;

impl Mutator for Inline_Asm_Faulty_Const_175 {
    fn name(&self) -> &str {
        "Inline_Asm_Faulty_Const_175"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if mac.mac.path.is_ident("global_asm") || mac.mac.path.is_ident("asm") {
                    let mut tokens = mac.mac.tokens.clone().into_iter();
                    let mut new_tokens = TokenStream::new();
                    while let Some(token) = tokens.next() {
                        if let TokenTree::Ident(ident) = &token {
                            if ident == "const" {
                                if let Some(TokenTree::Literal(lit)) = tokens.next() {
                                    let lit_str = lit.to_string();
                                    if let Ok(value) = lit_str.parse::<i32>() {
                                        let faulty_expr: Expr = parse_quote! { 1 / 0 };
                                        new_tokens.extend(quote! { const #faulty_expr });
                                        continue;
                                    }
                                }
                            }
                        }
                        new_tokens.extend(quote! { #token });
                    }
                    mac.mac.tokens = new_tokens;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly macros (`global_asm!` or `asm!`) and introduces faulty constant expressions, such as division by zero, within them. This transformation aims to provoke unexpected behavior during constant evaluation, potentially leading to internal compiler errors (ICEs) by stressing the compiler's handling of erroneous constant expressions in inline assembly contexts."
    }
}