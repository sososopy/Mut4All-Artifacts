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

pub struct Unicode_Parentheses_Replacement_13;

impl Mutator for Unicode_Parentheses_Replacement_13 {
    fn name(&self) -> &str {
        "Unicode_Parentheses_Replacement_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ParenReplacer;

        impl VisitMut for ParenReplacer {
            fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
                node.paren_token = syn::token::Paren(DelimSpan::call_site());
                syn::visit_mut::visit_expr_call_mut(self, node);
            }

            fn visit_expr_mut(&mut self, node: &mut Expr) {
                if let Expr::Paren(expr_paren) = node {
                    expr_paren.paren_token = syn::token::Paren(DelimSpan::call_site());
                }
                syn::visit_mut::visit_expr_mut(self, node);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                ParenReplacer.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces standard parentheses '(' and ')' within function calls and expressions with their full-width Unicode equivalents '（' and '）'. This transformation targets sections of code where parentheses are used to group expressions or pass arguments in function calls, potentially exposing bugs related to Unicode handling in the compiler's parser and error diagnostic components."
    }
}