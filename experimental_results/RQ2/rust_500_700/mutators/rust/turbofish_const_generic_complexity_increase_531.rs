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

pub struct Turbofish_Const_Generic_Complexity_Increase_531;

impl Mutator for Turbofish_Const_Generic_Complexity_Increase_531 {
    fn name(&self) -> &str {
        "Turbofish_Const_Generic_Complexity_Increase_531"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = TurbofishVisitor;
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets turbofish syntax with const generics, replacing simple const expressions with more complex arithmetic expressions. This increases the complexity of const generic parameters, potentially revealing issues in the compiler's const evaluation and type resolution processes."
    }
}

struct TurbofishVisitor;

impl VisitMut for TurbofishVisitor {
    fn visit_expr_path_mut(&mut self, expr_path: &mut ExprPath) {
        if let Some(last_segment) = expr_path.path.segments.last_mut() {
            if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(expr) = arg {
                        if let Expr::Lit(expr_lit) = expr {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                let value = lit_int.base10_parse::<usize>().unwrap_or(0);
                                let new_expr: Expr = parse_quote! { #value + 0 };
                                *expr = new_expr;
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_path_mut(self, expr_path);
    }
}