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

pub struct Remove_Const_Trait_Annotation_323;

impl Mutator for Remove_Const_Trait_Annotation_323 {
    fn name(&self) -> &str {
        "Remove_Const_Trait_Annotation_323"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_traits = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait
                    .attrs
                    .iter()
                    .any(|attr| attr.path().is_ident("const_trait"))
                {
                    const_traits.insert(item_trait.ident.clone());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                item_trait
                    .attrs
                    .retain(|attr| !attr.path().is_ident("const_trait"));
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            let mut new_bounds = Punctuated::new();
                            for bound in &predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(last_segment) =
                                        trait_bound.path.segments.last()
                                    {
                                        if const_traits.contains(&last_segment.ident) {
                                            let new_bound = TypeParamBound::Trait(
                                                TraitBound {
                                                    paren_token: trait_bound.paren_token,
                                                    modifier: TraitBoundModifier::Maybe(
                                                        token::Question {
                                                            spans: [Span::call_site()],
                                                        },
                                                    ),
                                                    lifetimes: trait_bound.lifetimes.clone(),
                                                    path: trait_bound.path.clone(),
                                                },
                                            );
                                            new_bounds.push(new_bound);
                                            continue;
                                        }
                                    }
                                }
                                new_bounds.push(bound.clone());
                            }
                            predicate_type.bounds = new_bounds;
                        }
                    }
                }
            }
            if let Item::Struct(item_struct) = item {
                if let Some(where_clause) = &mut item_struct.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            let mut new_bounds = Punctuated::new();
                            for bound in &predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(last_segment) =
                                        trait_bound.path.segments.last()
                                    {
                                        if const_traits.contains(&last_segment.ident) {
                                            let new_bound = TypeParamBound::Trait(
                                                TraitBound {
                                                    paren_token: trait_bound.paren_token,
                                                    modifier: TraitBoundModifier::Maybe(
                                                        token::Question {
                                                            spans: [Span::call_site()],
                                                        },
                                                    ),
                                                    lifetimes: trait_bound.lifetimes.clone(),
                                                    path: trait_bound.path.clone(),
                                                },
                                            );
                                            new_bounds.push(new_bound);
                                            continue;
                                        }
                                    }
                                }
                                new_bounds.push(bound.clone());
                            }
                            predicate_type.bounds = new_bounds;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits annotated with `#[const_trait]`, removing the attribute and then attempting to use the trait with `~const` bounds in const contexts like `const fn` and type definitions. By stripping the const trait annotation while maintaining const usage, it stresses the compiler's handling of const trait semantics, potentially exposing bugs in const trait resolution and const fn validation."
    }
}