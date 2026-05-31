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

pub struct Modify_Const_Fn_Return_Type_101;

impl Mutator for Modify_Const_Fn_Return_Type_101 {
    fn name(&self) -> &str {
        "Modify_Const_Fn_Return_Type_101"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    if let Some(where_clause) = &mut func.sig.generics.where_clause {
                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                for bound in &mut predicate_type.bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.is_ident("Sized") {
                                            trait_bound.modifier = TraitBoundModifier::Maybe(token::Not {
                                                bang_token: token::Bang {
                                                    spans: [Span::call_site()],
                                                },
                                            });
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
        ""
    }
}