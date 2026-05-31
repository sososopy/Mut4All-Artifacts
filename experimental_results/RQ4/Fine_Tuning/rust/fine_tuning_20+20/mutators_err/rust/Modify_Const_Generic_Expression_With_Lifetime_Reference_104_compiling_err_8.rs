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

pub struct Modify_Const_Generic_Expression_With_Lifetime_Reference_104;

impl Mutator for Modify_Const_Generic_Expression_With_Lifetime_Reference_104 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Expression_With_Lifetime_Reference_104"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_generic_fns = HashSet::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                let has_const_generic = item_fn
                    .sig
                    .generics
                    .params
                    .iter()
                    .any(|param| matches!(param, GenericParam::Const(_)));
                if has_const_generic {
                    const_generic_fns.insert(item_fn.sig.ident.to_string());
                }
            }
        }
        struct CallVisitor<'a> {
            const_generic_fns: &'a HashSet<String>,
            target_calls: Vec<usize>,
        }
        impl<'a> Visit<'_> for CallVisitor<'a> {
            fn visit_expr_call(&mut self, node: &ExprCall) {
                if let Expr::Path(expr_path) = &*node.func {
                    if expr_path.path.segments.len() == 1 {
                        let ident = &expr_path.path.segments.first().unwrap().ident;
                        if self.const_generic_fns.contains(&ident.to_string()) {
                            self.target_calls.push(node.span().start().line);
                        }
                    }
                }
                syn::visit::visit_expr_call(self, node);
            }
        }
        let mut visitor = CallVisitor {
            const_generic_fns: &const_generic_fns,
            target_calls: Vec::new(),
        };
        visitor.visit_file(file);
        struct Mutator {
            target_calls: HashSet<usize>,
            current_line: usize,
            current_lifetimes: HashSet<String>,
        }
        impl VisitMut for Mutator {
            fn visit_item_fn(&mut self, node: &mut ItemFn) {
                self.current_lifetimes.clear();
                for param in &node.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        self.current_lifetimes
                            .insert(lifetime_param.lifetime.ident.to_string());
                    }
                }
                syn::visit_mut::visit_item_fn(self, node);
            }
            fn visit_expr_call(&mut self, node: &mut ExprCall) {
                if self.target_calls.contains(&self.current_line) {
                    if let Expr::Path(expr_path) = &*node.func {
                        if expr_path.path.segments.len() == 1 {
                            let ident = &expr_path.path.segments.first().unwrap().ident;
                            if ident == "main" {
                                syn::visit_mut::visit_expr_call(self, node);
                                return;
                            }
                        }
                    }
                    let mut new_args = Punctuated::new();
                    for arg in &node.args {
                        if let Expr::Block(expr_block) = arg {
                            let mut new_block = expr_block.block.clone();
                            let mut has_lifetime = false;
                            for stmt in &new_block.stmts {
                                if let Stmt::Local(Local {
                                    pat,
                                    init: Some((_, init_expr)),
                                    ..
                                }) = stmt
                                {
                                    if let Pat::Wild(_) = pat {
                                        if let Expr::Reference(expr_ref) = &**init_expr {
                                            if let Type::Reference(type_ref) = &*expr_ref.expr {
                                                if let Some(lifetime) = &type_ref.lifetime {
                                                    has_lifetime = true;
                                                    self.current_lifetimes
                                                        .insert(lifetime.ident.to_string());
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if !has_lifetime {
                                let lifetime_ident = if self.current_lifetimes.contains("a") {
                                    let mut suffix = 1;
                                    loop {
                                        let candidate = format!("a{}", suffix);
                                        if !self.current_lifetimes.contains(&candidate) {
                                            self.current_lifetimes.insert(candidate.clone());
                                            break candidate;
                                        }
                                        suffix += 1;
                                    }
                                } else {
                                    self.current_lifetimes.insert("a".to_string());
                                    "a".to_string()
                                };
                                new_block.stmts.insert(
                                    0,
                                    parse_quote! {
                                        let _: &'#lifetime_ident ();
                                    },
                                );
                            }
                            new_args.push(Expr::Block(ExprBlock {
                                attrs: Vec::new(),
                                label: None,
                                block: new_block,
                            }));
                        } else {
                            new_args.push(arg.clone());
                        }
                    }
                    node.args = new_args;
                }
                syn::visit_mut::visit_expr_call(self, node);
            }
            fn visit_expr(&mut self, node: &mut Expr) {
                if node.span().start().line != self.current_line {
                    self.current_line = node.span().start().line;
                }
                syn::visit_mut::visit_expr(self, node);
            }
        }
        let mut mutator = Mutator {
            target_calls: visitor.target_calls.into_iter().collect(),
            current_line: 0,
            current_lifetimes: HashSet::new(),
        };
        mutator.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets function calls to functions with const generics and modifies the const generic arguments to include a lifetime-bound reference. It identifies calls to const generic functions, checks if the argument is a block expression, and injects a lifetime-bound reference if not already present. This transformation leverages the interplay between const generics and lifetime resolution to stress-test the compiler's handling of complex const expressions, potentially exposing bugs in const evaluation, lifetime inference, and generic resolution."
    }
}