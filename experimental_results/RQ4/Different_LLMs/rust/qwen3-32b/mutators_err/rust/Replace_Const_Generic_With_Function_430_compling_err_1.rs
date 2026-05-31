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

pub struct Replace_Const_Generic_With_Function_430;

impl Mutator for Replace_Const_Generic_With_Function_430 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Function_430"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut collector = FunctionCollector::new();
        collector.visit_file(file);
        let functions: Vec<_> = collector.functions.into_iter().collect();
        if functions.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let mut visitor = ReplaceConstArgsVisitor::new(&functions, &mut rng);
        visitor.visit_file(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces valid const generic parameters with existing non-const function names in const generic positions. This introduces semantically invalid but syntactically valid code, potentially exposing ICEs in const generic analysis by forcing the compiler to evaluate non-const functions as const arguments."
    }
}

struct FunctionCollector {
    functions: HashSet<Ident>,
}

impl FunctionCollector {
    fn new() -> Self {
        Self {
            functions: HashSet::new(),
        }
    }
}

impl Visit for FunctionCollector {
    fn visit_item_fn(&mut self, i: &ItemFn) {
        if i.constness.is_none() {
            self.functions.insert(i.sig.ident.clone());
        }
        syn::visit::visit_item_fn(self, i);
    }
}

struct ReplaceConstArgsVisitor<'a> {
    functions: &'a Vec<Ident>,
    rng: &'a mut ThreadRng,
}

impl<'a> ReplaceConstArgsVisitor<'a> {
    fn new(functions: &'a Vec<Ident>, rng: &'a mut ThreadRng) -> Self {
        Self { functions, rng }
    }

    fn replace_expr_with_func(&self, expr: &mut Expr) {
        if let Some(func) = self.functions.choose(self.rng) {
            *expr = parse_quote!(#func);
        }
    }
}

impl<'a> VisitMut for ReplaceConstArgsVisitor<'a> {
    fn visit_expr_array_mut(&mut self, node: &mut ExprArray) {
        if let Some(ref mut len) = node.len {
            self.replace_expr_with_func(len);
        }
        syn::visit_mut::visit_expr_array_mut(self, node);
    }

    fn visit_path_mut(&mut self, node: &mut SynPath) {
        if let Some(segment) = node.segments.last_mut() {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(expr) = arg {
                        self.replace_expr_with_func(expr);
                    }
                }
            }
        }
        syn::visit_mut::visit_path_mut(self, node);
    }
}