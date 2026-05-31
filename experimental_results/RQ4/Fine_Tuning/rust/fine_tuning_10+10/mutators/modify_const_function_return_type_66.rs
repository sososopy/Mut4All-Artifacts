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

pub struct Modify_Const_Function_Return_Type_66;

impl Mutator for Modify_Const_Function_Return_Type_66 {
    fn name(&self) -> &str {
        "Modify_Const_Function_Return_Type_66"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                for bound in &mut predicate_type.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.modifier == TraitBoundModifier::None {
                                            if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                                                if last_segment.ident == "Sized" {
                                                    last_segment.ident = Ident::new("Unsized", last_segment.ident.span());
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets const function declarations to modify the trait bounds in the where clause. Specifically, it changes a Sized bound to Unsized, introducing a logically incorrect condition. This mutation can help test the compiler's robustness in handling trait bounds in constant expressions."
    }
}