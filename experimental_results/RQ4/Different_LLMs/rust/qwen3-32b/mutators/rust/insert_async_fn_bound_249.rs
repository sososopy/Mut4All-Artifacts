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

pub struct Insert_Async_Fn_Bound_249;

impl Mutator for Insert_Async_Fn_Bound_249 {
    fn name(&self) -> &str {
        "Insert_Async_Fn_Bound_249"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                self.process_function_generics(&mut func.sig.generics);
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(type_pred) = predicate {
                            for bound in &mut type_pred.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    self.process_trait_bound(trait_bound);
                                }
                            }
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        self.process_function_generics(&mut method.sig.generics);
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(type_pred) = predicate {
                                    for bound in &mut type_pred.bounds {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            self.process_trait_bound(trait_bound);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts 'async' before Fn, FnMut, or FnOnce trait bounds in function and method parameters. This transformation introduces unstable async closures, potentially triggering SyntheticCoroutineBody processing and ICEs during MIR dump when the compiler encounters unhandled coroutine definitions."
    }
}

impl Insert_Async_Fn_Bound_249 {
    fn process_function_generics(&self, generics: &mut syn::Generics) {
        for param in &mut generics.params {
            if let syn::GenericParam::Type(type_param) = param {
                for bound in &mut type_param.bounds {
                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                        self.process_trait_bound(trait_bound);
                    }
                }
            }
        }
    }

    fn process_trait_bound(&self, trait_bound: &mut TraitBound) {
        if let Some(first_segment) = trait_bound.path.segments.first() {
            if first_segment.ident == "Fn" || first_segment.ident == "FnMut" || first_segment.ident == "FnOnce" {
                let new_segment = syn::PathSegment {
                    ident: syn::Ident::new("Fn", first_segment.ident.span()),
                    arguments: first_segment.arguments.clone(),
                };
                let mut segments = Punctuated::new();
                segments.push(syn::PathSegment {
                    ident: syn::Ident::new("async", first_segment.ident.span()),
                    arguments: syn::PathArguments::None,
                });
                segments.push(new_segment);
                trait_bound.path = syn::Path {
                    leading_colon: None,
                    segments,
                };
            }
        }
    }
}