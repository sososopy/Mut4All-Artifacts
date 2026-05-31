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
            if let Item::Fn(item_fn) = item {
                let mut visitor = UnicodeCharReplacer;
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator replaces standard ASCII characters with visually similar Unicode characters in function calls and expressions, aiming to expose bugs related to Unicode handling in the Rust compiler."
    }
}

struct UnicodeCharReplacer;

impl VisitMut for UnicodeCharReplacer {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        for arg in &mut node.args {
            self.visit_expr_mut(arg);
        }
        if let Expr::Path(ExprPath { path: SynPath { segments, .. }, .. }) = &*node.func {
            if let Some(last_segment) = segments.last() {
                if last_segment.ident == "calculate" {
                    let mut new_args = Punctuated::new();
                    for arg in &node.args {
                        if let Expr::Lit(expr_lit) = arg {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                let value = lit_int.base10_parse::<i32>().unwrap();
                                let new_arg: Expr = parse_quote! { #value };
                                new_args.push(new_arg);
                            }
                        }
                    }
                    node.args = new_args;
                }
            }
        }
        node.paren_token = token::Paren::default();
    }
}