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

pub struct Swap_Nested_Closure_Lifetime_Binders_40;

impl Mutator for Swap_Nested_Closure_Lifetime_Binders_40 {
    fn name(&self) -> &str {
        "Swap_Nested_Closure_Lifetime_Binders_40"
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator swaps lifetime binders in nested closures by identifying lifetimes in closure arguments and swapping them."
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = SwapNestedClosureLifetimeBindersVisitor;
        visitor.visit_file_mut(file);
    }
}

struct SwapNestedClosureLifetimeBindersVisitor;

impl VisitMut for SwapNestedClosureLifetimeBindersVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            if let Some((inner_lifetime, outer_lifetime)) = self.find_nested_lifetime_binders(&closure) {
                self.swap_lifetime_binders(expr, inner_lifetime, outer_lifetime);
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

impl SwapNestedClosureLifetimeBindersVisitor {
    fn find_nested_lifetime_binders(&self, closure: &syn::ExprClosure) -> Option<(Lifetime, Lifetime)> {
        let mut lifetimes = Vec::new();
        for arg in &closure.inputs {
            if let Pat::Type(PatType { ty, .. }) = arg {
                self.collect_lifetimes_from_type(&ty, &mut lifetimes);
            }
        }
        if lifetimes.len() >= 2 {
            let mut rng = thread_rng();
            let inner_idx = rng.gen_range(0..lifetimes.len());
            let outer_idx = rng.gen_range(0..lifetimes.len());
            if inner_idx != outer_idx {
                return Some((lifetimes[inner_idx], lifetimes[outer_idx]));
            }
        }
        None
    }

    fn collect_lifetimes_from_type(&self, ty: &Type, lifetimes: &mut Vec<Lifetime>) {
        match ty {
            Type::Path(TypePath { path, .. }) => {
                for segment in &path.segments {
                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                        for arg in &args.args {
                            if let GenericArgument::Lifetime(lifetime) = arg {
                                lifetimes.push(lifetime.clone());
                            }
                        }
                    }
                }
            }
            Type::ImplTrait(TypeImplTrait { bounds, .. }) => {
                for bound in bounds {
                    if let TypeParamBound::Trait(TraitBound { path, .. }) = bound {
                        for segment in &path.segments {
                            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                for arg in &args.args {
                                    if let GenericArgument::Lifetime(lifetime) = arg {
                                        lifetimes.push(lifetime.clone());
                                    }
                                }
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }

    fn swap_lifetime_binders(&self, expr: &mut Expr, inner: Lifetime, outer: Lifetime) {
        if let Expr::Closure(closure) = expr {
            for arg in &mut closure.inputs {
                if let Pat::Type(PatType { ty, .. }) = arg {
                    self.swap_lifetimes_in_type(&mut ty, &inner, &outer);
                }
            }
        }
    }

    fn swap_lifetimes_in_type(&self, ty: &mut Type, inner: &Lifetime, outer: &Lifetime) {
        match ty {
            Type::Path(TypePath { path, .. }) => {
                for segment in &mut path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Lifetime(lifetime) = arg {
                                if lifetime == inner {
                                    *lifetime = outer.clone();
                                } else if lifetime == outer {
                                    *lifetime = inner.clone();
                                }
                            }
                        }
                    }
                }
            }
            Type::ImplTrait(TypeImplTrait { bounds, .. }) => {
                for bound in bounds {
                    if let TypeParamBound::Trait(TraitBound { path, .. }) = bound {
                        for segment in &mut path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Lifetime(lifetime) = arg {
                                        if lifetime == inner {
                                            *lifetime = outer.clone();
                                        } else if lifetime == outer {
                                            *lifetime = inner.clone();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }
}