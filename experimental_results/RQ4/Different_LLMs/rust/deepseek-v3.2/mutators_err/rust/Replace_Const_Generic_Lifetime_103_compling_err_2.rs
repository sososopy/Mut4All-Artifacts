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

pub struct Replace_Const_Generic_Lifetime_103;

impl Mutator for Replace_Const_Generic_Lifetime_103 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Lifetime_103"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut available_lifetimes: Vec<Lifetime> = Vec::new();
                for param in &func.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        available_lifetimes.push(lifetime_param.lifetime.clone());
                    }
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let mut new_type = const_param.ty.clone();
                        let mut changed = false;
                        if let Type::Reference(type_ref) = &mut new_type {
                            if let Some(ref mut lifetime) = &mut type_ref.lifetime {
                                let candidate_lifetimes = if available_lifetimes.is_empty() {
                                    vec![Lifetime::new("'static", Span::call_site())]
                                } else {
                                    available_lifetimes.clone()
                                };
                                if candidate_lifetimes.len() > 1 {
                                    let mut rng = thread_rng();
                                    let new_lifetime = candidate_lifetimes.choose(&mut rng).unwrap();
                                    if *new_lifetime != *lifetime {
                                        *lifetime = new_lifetime.clone();
                                        changed = true;
                                    }
                                } else if candidate_lifetimes.len() == 1 && candidate_lifetimes[0] != *lifetime {
                                    *lifetime = candidate_lifetimes[0].clone();
                                    changed = true;
                                }
                            }
                        }
                        if changed {
                            const_param.ty = new_type;
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut available_lifetimes: Vec<Lifetime> = Vec::new();
                        for param in &func.sig.generics.params {
                            if let GenericParam::Lifetime(lifetime_param) = param {
                                available_lifetimes.push(lifetime_param.lifetime.clone());
                            }
                        }
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                let mut new_type = const_param.ty.clone();
                                let mut changed = false;
                                if let Type::Reference(type_ref) = &mut new_type {
                                    if let Some(ref mut lifetime) = &mut type_ref.lifetime {
                                        let candidate_lifetimes = if available_lifetimes.is_empty() {
                                            vec![Lifetime::new("'static", Span::call_site())]
                                        } else {
                                            available_lifetimes.clone()
                                        };
                                        if candidate_lifetimes.len() > 1 {
                                            let mut rng = thread_rng();
                                            let new_lifetime = candidate_lifetimes.choose(&mut rng).unwrap();
                                            if *new_lifetime != *lifetime {
                                                *lifetime = new_lifetime.clone();
                                                changed = true;
                                            }
                                        } else if candidate_lifetimes.len() == 1 && candidate_lifetimes[0] != *lifetime {
                                            *lifetime = candidate_lifetimes[0].clone();
                                            changed = true;
                                        }
                                    }
                                }
                                if changed {
                                    const_param.ty = new_type;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters in function and method signatures that have reference types with explicit lifetime annotations. It collects all lifetime parameters declared in the same signature, then randomly selects a different lifetime from this set (or 'static if no other lifetimes exist) to replace the original lifetime in the const parameter's type. This transformation tests the compiler's handling of lifetime substitution in const generics, potentially causing lifetime mismatch errors, const evaluation failures, or unexpected behavior in generic contexts."
    }
}