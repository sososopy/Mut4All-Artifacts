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

pub struct Modify_Const_Fn_Return_Type_To_Invalid_Condition_101;

impl Mutator for Modify_Const_Fn_Return_Type_To_Invalid_Condition_101 {
    fn name(&self) -> &str {
        "Modify_Const_Fn_Return_Type_To_Invalid_Condition_101"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                let mut new_bounds = Punctuated::new();
                                for bound in &predicate_type.bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound
                                            .path
                                            .segments
                                            .last()
                                            .map_or(false, |seg| seg.ident == "Sized")
                                        {
                                            let new_trait_bound = syn::TraitBound {
                                                paren_token: trait_bound.paren_token.clone(),
                                                modifier:
                                                    syn::TraitBoundModifier::Maybe(syn::token::Not {
                                                        bang_token: syn::token::Bang::default(),
                                                    }),
                                                lifetimes: trait_bound.lifetimes.clone(),
                                                path: trait_bound.path.clone(),
                                            };
                                            new_bounds.push(syn::TypeParamBound::Trait(
                                                new_trait_bound,
                                            ));
                                        } else {
                                            new_bounds.push(bound.clone());
                                        }
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                }
                                predicate_type.bounds = new_bounds;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets const functions by modifying their where clause to replace `Sized` bounds with `!Sized`. This transformation introduces a logically invalid condition, which can lead to compilation errors or unexpected behavior, particularly in const contexts where trait bounds are crucial for type safety and inference."
    }
}