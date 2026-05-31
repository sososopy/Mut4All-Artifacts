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

pub struct Mutator_Replace_Closure_With_Lifetime_Binder_429;

impl Mutator for Mutator_Replace_Closure_With_Lifetime_Binder_429 {
    fn name(&self) -> &str {
        "Replace_Closure_With_Lifetime_Binder_429"
    }

    fn mutate(&self, file: &mut syn::File) {
        let has_feature = file.attrs.iter().any(|attr| {
            if let syn::Meta::List(list) = &attr.meta {
                if list.path.is_ident("feature") {
                    let mut nested = Vec::new();
                    syn::parse2::<syn::Meta>(list.tokens.clone()).map(|n| nested.push(n)).ok();
                    for item in nested {
                        if let syn::Meta::Path(path) = item {
                            if path.segments.iter().any(|seg| seg.ident == "closure_lifetime_binder") {
                                return true;
                            }
                        }
                    }
                }
            }
            false
        });

        if !has_feature {
            let feature_attr = parse_quote! { #![feature(closure_lifetime_binder)] };
            file.attrs.insert(0, feature_attr);
        }

        struct ClosureReplacer;
        impl VisitMut for ClosureReplacer {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Const(expr_const) = expr {
                    for stmt in &mut expr_const.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Closure(closure) = expr.as_mut() {
                                let new_closure = parse_quote! {
                                    for<'a> |x: &'a i32| -> &'a i32 { x }
                                };
                                *expr = new_closure;
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut replacer = ClosureReplacer;
        replacer.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        ""
    }
}