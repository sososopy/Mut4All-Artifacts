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

pub struct Insert_Unused_Lifetime_For_Const_Fn_Bound_395;

impl Mutator for Insert_Unused_Lifetime_For_Const_Fn_Bound_395 {
    fn name(&self) -> &str {
        "Insert_Unused_Lifetime_For_Const_Fn_Bound_395"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.modifier == syn::TraitBoundModifier::Const && trait_bound.lifetimes.is_some() {
                                    let bound_lifetimes = trait_bound.lifetimes.as_mut().unwrap();
                                    let new_lt = syn::LifetimeParam {
                                        attrs: Vec::new(),
                                        lifetime: syn::Lifetime::new("'b", proc_macro2::Span::call_site()),
                                        colon_token: None,
                                    };
                                    bound_lifetimes.lifetimes.push(new_lt);
                                }
                            }
                        }
                    }
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(pred_type) = predicate {
                            for bound in &mut pred_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.modifier == syn::TraitBoundModifier::Const && trait_bound.lifetimes.is_some() {
                                        let bound_lifetimes = trait_bound.lifetimes.as_mut().unwrap();
                                        let new_lt = syn::LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: syn::Lifetime::new("'b", proc_macro2::Span::call_site()),
                                            colon_token: None,
                                        };
                                        bound_lifetimes.lifetimes.push(new_lt);
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
        ""
    }
}