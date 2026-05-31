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

pub struct Remove_Slice_Binding_In_Const_Closure_307;

impl Mutator for Remove_Slice_Binding_In_Const_Closure_307 {
    fn name(&self) -> &str {
        "Remove_Slice_Binding_In_Const_Closure_307"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                // Check if function is const and returns impl ~const Fn()
                let is_const = item_fn.sig.constness.is_some();
                let returns_const_fn = if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let bounds = &type_impl_trait.bounds;
                        bounds.iter().any(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let path = &trait_bound.path;
                                if path.segments.len() == 1 && path.segments[0].ident == "Fn" {
                                    trait_bound.modifier == TraitBoundModifier::None && trait_bound.lifetimes.is_none()
                                } else {
                                    false
                                }
                            } else {
                                false
                            }
                        })
                    } else {
                        false
                    }
                } else {
                    false
                };
                
                if is_const && returns_const_fn {
                    // Visit the function body to find match patterns with @ binding on slice patterns
                    let mut visitor = MatchPatternVisitor::new();
                    visitor.visit_item_fn_mut(item_fn);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const functions that return impl ~const Fn() and contain closures with match expressions on slice patterns that use @ bindings (e.g., [first, remainder @ ..]). It removes the @ binding, transforming the pattern to [first, ..]. This alters lifetime and const qualification analysis within the closure, potentially triggering compiler bugs related to const trait impl and closure handling."
    }
}

struct MatchPatternVisitor {
    // Helper struct to visit and mutate match patterns
}

impl MatchPatternVisitor {
    fn new() -> Self {
        Self {}
    }
}

impl VisitMut for MatchPatternVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Match(expr_match) = expr {
            for arm in &mut expr_match.arms {
                if let Pat::Slice(pat_slice) = &mut arm.pat {
                    let elems = &mut pat_slice.elems;
                    if elems.len() >= 2 {
                        // Check if the last element is a Pat::Rest with a binding
                        if let Pat::Rest(pat_rest) = &mut elems[elems.len() - 1] {
                            // Look for binding pattern: remainder @ ..
                            // In syn, Pat::Rest doesn't directly contain binding, but we need to check if it's wrapped in Pat::Ident with binding
                            // Actually, pattern like [first, remainder @ ..] is represented as:
                            // Pat::Slice with elems: [Pat::Ident(first), Pat::Rest(Pat::Ident(remainder))]
                            // We need to transform Pat::Rest(Pat::Ident(remainder)) to Pat::Rest(Pat::Wild)
                            if let Pat::Ident(pat_ident) = &mut pat_rest.rest {
                                // Remove the binding by replacing with wild pattern
                                pat_rest.rest = Pat::Wild(PatWild {
                                    attrs: Vec::new(),
                                    underscore_token: token::Underscore::default(),
                                });
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
    
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        syn::visit_mut::visit_item_fn_mut(self, i);
    }
}