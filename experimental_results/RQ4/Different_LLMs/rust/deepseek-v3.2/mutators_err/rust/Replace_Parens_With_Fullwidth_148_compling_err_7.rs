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

pub struct Replace_Parens_With_Fullwidth_148;

impl Mutator for Replace_Parens_With_Fullwidth_148 {
    fn name(&self) -> &str {
        "Replace_Parens_With_Fullwidth_148"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ParenReplacer;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function call expressions and replaces ASCII parentheses '(' and ')' with their fullwidth Unicode counterparts '（' and '）'. This transformation changes token spans without altering syntactic structure, potentially causing mismatches between byte positions and compiler span accounting. It aims to trigger assertion failures in source map or diagnostic subsystems by introducing Unicode tokens with different widths."
    }
}

struct ParenReplacer;

impl VisitMut for ParenReplacer {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        let paren_token = &mut node.paren_token;
        let mut new_left = proc_macro2::Punct::new('（', proc_macro2::Spacing::Alone);
        new_left.set_span(paren_token.span.join());
        let mut new_right = proc_macro2::Punct::new('）', proc_macro2::Spacing::Alone);
        new_right.set_span(paren_token.span.join());
        paren_token.span = proc_macro2::extra::DelimSpan::join(new_left.span(), new_right.span());
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
}