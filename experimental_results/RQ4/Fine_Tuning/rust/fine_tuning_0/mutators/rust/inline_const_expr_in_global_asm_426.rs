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

pub struct Inline_Const_Expr_In_Global_Asm_426;

impl Mutator for Inline_Const_Expr_In_Global_Asm_426 {
    fn name(&self) -> &str {
        "Inline_Const_Expr_In_Global_Asm_426"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if item_macro.mac.path.is_ident("global_asm") {
                    let mut tokens = item_macro.mac.tokens.clone().into_iter().peekable();
                    let mut new_tokens = TokenStream::new();
                    while let Some(token) = tokens.next() {
                        if let TokenTree::Ident(ident) = &token {
                            if ident == "const" {
                                if let Some(TokenTree::Literal(lit)) = tokens.peek() {
                                    if let Ok(expr) = syn::parse_str::<Expr>(&lit.to_string()) {
                                        if let Expr::Binary(expr_binary) = expr {
                                            if let syn::BinOp::Div(_) = expr_binary.op {
                                                let new_expr: Expr = parse_quote! { 1 };
                                                new_tokens.extend(quote! { const #expr_binary.left / #new_expr });
                                                tokens.next();
                                                continue;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        new_tokens.extend(quote! { #token });
                    }
                    item_macro.mac.tokens = new_tokens;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `global_asm!` macros with inline constant expressions involving division. It replaces the divisor with a non-zero constant to prevent division by zero errors while testing the handling of inline constant expressions in `global_asm!` macros."
    }
}