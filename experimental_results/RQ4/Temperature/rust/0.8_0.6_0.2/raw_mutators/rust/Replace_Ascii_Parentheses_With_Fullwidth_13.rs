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
        "The mutation operator replaces ASCII parentheses in function calls with fullwidth Unicode parentheses. This transformation tests the compiler's handling of Unicode characters that resemble standard ASCII characters, potentially exposing parsing or character recognition issues."
    }
}

struct FunctionCallVisitor;

impl VisitMut for FunctionCallVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        let args_str = node.args.iter()
            .map(|arg| quote! { #arg }.to_string())
            .collect::<Vec<_>>()
            .join(", ");
        
        let func_str = quote! { #node.func }.to_string();
        let new_call = format!("{}（{}）", func_str, args_str);
        
        *node = parse_quote! { #new_call };
    }
}