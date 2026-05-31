use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Array_Generic_Expr_349;

impl Mutator for Replace_Array_Generic_Expr_349 {
    fn name(&self) -> &str {
        "Replace_Array_Generic_Expr_349"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    // Add generic const parameter
                    let new_param = parse_quote!(const N: usize);
                    func.sig.generics.params.push(new_param);
                    // Replace array sizes in the function's block
                    let mut visitor = ArrayReplacer {};
                    visitor.visit_block_mut(&mut func.block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions and replaces fixed-size arrays with a generic const expression using a newly introduced const parameter. This transformation stresses the interaction between generic const expressions and async functions with lifetimes, potentially exposing ICEs or type resolution bugs in the compiler's handling of const generics in async contexts."
    }
}

struct ArrayReplacer;

impl VisitMut for ArrayReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Repeat(expr_repeat) = expr {
            // Replace the repeat expression's length with { N + 2 }
            let new_len = parse_quote! {{ N + 2 }};
            expr_repeat.len = Box::new(new_len);
        }
        // Continue visiting children
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}