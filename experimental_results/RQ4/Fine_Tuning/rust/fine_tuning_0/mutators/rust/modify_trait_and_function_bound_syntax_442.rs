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

pub struct Modify_Trait_And_Function_Bound_Syntax_442;

impl Mutator for Modify_Trait_And_Function_Bound_Syntax_442 {
    fn name(&self) -> &str {
        "Modify_Trait_And_Function_Bound_Syntax_442"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if predicate_type.bounds.iter().any(|bound| matches!(bound, syn::TypeParamBound::Trait(_))) {
                                predicate_type.bounds.push(parse_quote!('static));
                            }
                        }
                    }
                }
                for param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        type_param.bounds.push(parse_quote!('static));
                    }
                }
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &mut **ty {
                        type_impl_trait.bounds.push(parse_quote!('static));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    if predicate_type.bounds.iter().any(|bound| matches!(bound, syn::TypeParamBound::Trait(_))) {
                                        predicate_type.bounds.push(parse_quote!('static));
                                    }
                                }
                            }
                        }
                        for param in &mut func.sig.generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                type_param.bounds.push(parse_quote!('static));
                            }
                        }
                        if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                            if let syn::Type::ImplTrait(type_impl_trait) = &mut **ty {
                                type_impl_trait.bounds.push(parse_quote!('static));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}