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

pub struct Modify_Const_Generic_Arguments_In_Function_Calls_90;

impl Mutator for Modify_Const_Generic_Arguments_In_Function_Calls_90 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Arguments_In_Function_Calls_90"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_functions = HashSet::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                let has_const_generics = item_fn
                    .sig
                    .generics
                    .params
                    .iter()
                    .any(|param| matches!(param, GenericParam::Const(_)));
                if has_const_generics {
                    target_functions.insert(item_fn.sig.ident.to_string());
                }
            }
        }
        struct CallVisitor<'a> {
            target_functions: &'a HashSet<String>,
            calls: Vec<usize>,
        }
        impl<'a, 'ast> Visit<'ast> for CallVisitor<'a> {
            fn visit_expr_call(&mut self, node: &'ast ExprCall) {
                if let Expr::Path(ExprPath { path, .. }) = &*node.func {
                    if let Some(ident) = path.get_ident() {
                        if self.target_functions.contains(&ident.to_string()) {
                            self.calls.push(node.span().start().line);
                        }
                    }
                }
                syn::visit::visit_expr_call(self, node);
            }
        }
        let mut visitor = CallVisitor {
            target_functions: &target_functions,
            calls: Vec::new(),
        };
        visitor.visit_file(file);
        struct CallMutator {
            target_lines: HashSet<usize>,
        }
        impl VisitMut for CallMutator {
            fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
                if self.target_lines.contains(&node.span().start().line) {
                    for arg in &mut node.args {
                        if let Expr::Lit(expr_lit) = arg {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                if let Ok(value) = lit_int.base10_parse::<usize>() {
                                    if value < usize::MAX {
                                        *arg = parse_quote!(usize::MAX);
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, node);
            }
        }
        let mut mutator = CallMutator {
            target_lines: visitor.calls.into_iter().collect(),
        };
        mutator.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function calls that use const generics by identifying functions with const generic parameters and modifying their call sites. It replaces const generic arguments with `usize::MAX` to stress-test the compiler's const evaluation and type-checking mechanisms, potentially triggering bugs related to const generics handling and boundary value processing."
    }
}