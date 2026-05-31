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
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = SwapNestedClosureLifetimeBinderVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets nested closures with explicit lifetime binders. It swaps the usage of outer binder lifetime parameters inside inner closure signatures with the inner binder's own lifetime parameters, or vice versa. This creates mismatched binder scopes that can trigger normalization errors and expose bugs in the compiler's handling of binder region interactions."
    }
}

struct SwapNestedClosureLifetimeBinderVisitor;

impl VisitMut for SwapNestedClosureLifetimeBinderVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(outer_closure) = expr {
            if let Some(outer_binder) = &outer_closure.lifetimes {
                let outer_lifetimes: Vec<&LifetimeParam> = outer_binder.lifetimes.iter().filter_map(|gp| {
                    if let GenericParam::Lifetime(lp) = gp {
                        Some(lp)
                    } else {
                        None
                    }
                }).collect();
                if !outer_lifetimes.is_empty() {
                    let mut inner_visitor = InnerClosureVisitor {
                        outer_lifetimes: outer_lifetimes.iter().map(|lp| lp.lifetime.clone()).collect(),
                    };
                    inner_visitor.visit_expr_mut(&mut outer_closure.body);
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

struct InnerClosureVisitor {
    outer_lifetimes: Vec<Lifetime>,
}

impl VisitMut for InnerClosureVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(inner_closure) = expr {
            if let Some(inner_binder) = &inner_closure.lifetimes {
                let inner_lifetimes: Vec<&LifetimeParam> = inner_binder.lifetimes.iter().filter_map(|gp| {
                    if let GenericParam::Lifetime(lp) = gp {
                        Some(lp)
                    } else {
                        None
                    }
                }).collect();
                if !inner_lifetimes.is_empty() {
                    let inner_lifetime_names = inner_lifetimes.iter().map(|lp| lp.lifetime.clone()).collect::<Vec<Lifetime>>();
                    let mut rng = thread_rng();
                    
                    if let Some(outer_lifetime) = self.outer_lifetimes.choose(&mut rng) {
                        if let Some(inner_lifetime) = inner_lifetime_names.choose(&mut rng) {
                            let mut swapped = false;
                            for input in inner_closure.inputs.iter_mut() {
                                if let Pat::Type(PatType { ty, .. }) = input {
                                    swap_lifetime_in_type(ty, outer_lifetime, inner_lifetime);
                                    swapped = true;
                                }
                            }
                            if let ReturnType::Type(_, return_type) = &mut inner_closure.output {
                                swap_lifetime_in_type(return_type, outer_lifetime, inner_lifetime);
                                swapped = true;
                            }
                            if !swapped {
                                if let Some(input) = inner_closure.inputs.first_mut() {
                                    if let Pat::Type(PatType { ty, .. }) = input {
                                        introduce_lifetime_in_type(ty, outer_lifetime);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

fn swap_lifetime_in_type(ty: &mut Box<Type>, outer_lifetime: &Lifetime, inner_lifetime: &Lifetime) {
    match &mut **ty {
        Type::Reference(ref_type) => {
            if let Some(ref_lifetime) = &mut ref_type.lifetime {
                if ref_lifetime.ident == outer_lifetime.ident {
                    ref_lifetime.ident = inner_lifetime.ident.clone();
                } else if ref_lifetime.ident == inner_lifetime.ident {
                    ref_lifetime.ident = outer_lifetime.ident.clone();
                }
            }
        }
        Type::Path(type_path) => {
            for segment in &mut type_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Lifetime(lifetime_arg) = arg {
                            if lifetime_arg.ident == outer_lifetime.ident {
                                lifetime_arg.ident = inner_lifetime.ident.clone();
                            } else if lifetime_arg.ident == inner_lifetime.ident {
                                lifetime_arg.ident = outer_lifetime.ident.clone();
                            }
                        }
                    }
                }
            }
        }
        _ => {}
    }
}

fn introduce_lifetime_in_type(ty: &mut Box<Type>, lifetime: &Lifetime) {
    match &mut **ty {
        Type::Reference(ref_type) => {
            if ref_type.lifetime.is_none() {
                ref_type.lifetime = Some(lifetime.clone());
            }
        }
        Type::Path(type_path) => {
            if let Some(segment) = type_path.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    args.args.push(GenericArgument::Lifetime(lifetime.clone()));
                } else {
                    segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                        colon2_token: None,
                        lt_token: token::Lt::default(),
                        args: Punctuated::from_iter(vec![GenericArgument::Lifetime(lifetime.clone())]),
                        gt_token: token::Gt::default(),
                    });
                }
            }
        }
        _ => {}
    }
}