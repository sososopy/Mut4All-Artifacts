use proc_macro2::{Span, TokenStream};
use proc_macro2::extra::DelimSpan;
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
use proc_macro2::Delimiter;

use crate::mutator::Mutator;

pub struct Replace_Ascii_Parentheses_With_Fullwidth_13;

impl Mutator for Replace_Ascii_Parentheses_With_Fullwidth_13 {
    fn name(&self) -> &str {
        "Replace_Ascii_Parentheses_With_Fullwidth_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = FunctionCallVisitor;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function calls and replaces ASCII parentheses with fullwidth Unicode parentheses. This transformation tests the compiler's handling of Unicode characters in parsing and tokenization, potentially revealing bugs in Unicode support."
    }
}

struct FunctionCallVisitor;

impl VisitMut for FunctionCallVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        node.paren_token = token::Paren {
            span: DelimSpan::from_single(Span::call_site()),
        };
        for arg in &mut node.args {
            self.visit_expr_mut(arg);
        }
    }
}