use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Modify_Inline_Assembly_Operand_Constraints_584;

impl Mutator for Modify_Inline_Assembly_Operand_Constraints_584 {
    fn name(&self) -> &str {
        "Modify_Inline_Assembly_Operand_Constraints_584"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Unsafe(expr_unsafe) = expr {
                            for inner_stmt in &mut expr_unsafe.block.stmts {
                                if let Stmt::Semi(expr, _) = inner_stmt {
                                    if let Expr::Macro(expr_macro) = expr {
                                        if expr_macro.mac.path.is_ident("asm") {
                                            let mut tokens = expr_macro.mac.tokens.clone().into_iter();
                                            let mut new_tokens = TokenStream::new();
                                            while let Some(token) = tokens.next() {
                                                new_tokens.extend(Some(token.clone()));
                                                if let TokenTree::Punct(punct) = &token {
                                                    if punct.as_char() == '=' || punct.as_char() == '+' {
                                                        if let Some(TokenTree::Ident(ident)) = tokens.next() {
                                                            if ident == "m" || ident == "r" {
                                                                let mut rng = thread_rng();
                                                                let options = match ident.to_string().as_str() {
                                                                    "m" => vec!["=m", "+m", "m"],
                                                                    "r" => vec!["=r", "+r", "r"],
                                                                    _ => vec![],
                                                                };
                                                                if let Some(new_constraint) = options.choose(&mut rng) {
                                                                    new_tokens.extend(Some(TokenTree::Ident(Ident::new(new_constraint, ident.span()))));
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            expr_macro.mac.tokens = new_tokens;
                                        }
                                    }
                                }
                            }
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