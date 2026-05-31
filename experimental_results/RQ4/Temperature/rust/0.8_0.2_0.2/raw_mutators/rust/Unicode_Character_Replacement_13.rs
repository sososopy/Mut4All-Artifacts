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

pub struct Unicode_Character_Replacement_13;

impl Mutator for Unicode_Character_Replacement_13 {
    fn name(&self) -> &str {
        "Unicode_Character_Replacement_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = UnicodeParenthesesReplacer;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator replaces Unicode characters that resemble standard ASCII parentheses in function calls and expressions with their ASCII equivalents. This ensures that the compiler correctly interprets these characters, testing its ability to handle Unicode character recognition and tokenization."
    }
}

struct UnicodeParenthesesReplacer;

impl VisitMut for UnicodeParenthesesReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(expr_call) = expr {
            replace_unicode_parentheses(&mut expr_call.args);
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

fn replace_unicode_parentheses(args: &mut Punctuated<Expr, Comma>) {
    for arg in args.iter_mut() {
        if let Expr::Path(expr_path) = arg {
            if let Some(last_segment) = expr_path.path.segments.last_mut() {
                if last_segment.ident == "（" {
                    last_segment.ident = Ident::new("(", Span::call_site());
                } else if last_segment.ident == "）" {
                    last_segment.ident = Ident::new(")", Span::call_site());
                }
            }
        }
    }
}