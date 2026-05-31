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

pub struct Mutable_Argument_Check_Async_649;

impl Mutator for Mutable_Argument_Check_Async_649 {
    fn name(&self) -> &str {
        "Mutable_Argument_Check_Async_649"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            let mut requires_mut = false;
                            let mut visitor = MutabilityChecker {
                                param_ident: &pat_type.pat,
                                requires_mut: &mut requires_mut,
                            };
                            visitor.visit_block(&func.block);
                            if *visitor.requires_mut {
                                if let Pat::Ident(pat_ident) = &mut *pat_type.pat {
                                    pat_ident.mutability = Some(token::Mut {
                                        span: Span::call_site(),
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions and checks if any parameters are used in a mutable context. If a parameter is found to be used in a way that requires mutability, it modifies the parameter to be mutable. This ensures that the function does not encounter compilation errors due to improper mutability handling of parameters."
    }
}

struct MutabilityChecker<'a> {
    param_ident: &'a Box<Pat>,
    requires_mut: &'a mut bool,
}

impl<'a> Visit<'a> for MutabilityChecker<'a> {
    fn visit_expr_call(&mut self, node: &'a ExprCall) {
        if let Expr::Path(ExprPath { path: syn_path, .. }) = &*node.func {
            if syn_path.segments.last().unwrap().ident == "push" {
                for arg in &node.args {
                    if let Expr::Path(ExprPath { path, .. }) = arg {
                        if let Some(ident) = path.get_ident() {
                            if let Pat::Ident(pat_ident) = &**self.param_ident {
                                if ident == &pat_ident.ident {
                                    *self.requires_mut = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        visit::visit_expr_call(self, node);
    }

    fn visit_expr_assign(&mut self, node: &'a ExprAssign) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.left {
            if let Some(ident) = path.get_ident() {
                if let Pat::Ident(pat_ident) = &**self.param_ident {
                    if ident == &pat_ident.ident {
                        *self.requires_mut = true;
                    }
                }
            }
        }
        visit::visit_expr_assign(self, node);
    }
}