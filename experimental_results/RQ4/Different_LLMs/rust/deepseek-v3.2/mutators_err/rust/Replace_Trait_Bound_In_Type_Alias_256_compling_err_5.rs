use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, PathSegment, PredicateType, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, WherePredicate, parse_quote,
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

pub struct Replace_Trait_Bound_In_Type_Alias_256;

impl Mutator for Replace_Trait_Bound_In_Type_Alias_256 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_In_Type_Alias_256"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(type_alias) = item {
                if let Type::Path(type_path) = &*type_alias.ty {
                    let mut modified = false;
                    let mut new_bounds = Punctuated::<TypeParamBound, Plus>::new();
                    if let Some(where_clause) = &type_alias.generics.where_clause {
                        for predicate in &where_clause.predicates {
                            if let WherePredicate::Type(predicate) = predicate {
                                for bound in &predicate.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if !modified {
                                            let mut path = trait_bound.path.clone();
                                            if path.segments.len() > 1 {
                                                path.segments.pop();
                                            } else {
                                                path.segments = Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("_", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]);
                                            }
                                            new_bounds.push(TypeParamBound::Trait(TraitBound {
                                                paren_token: trait_bound.paren_token,
                                                modifier: trait_bound.modifier,
                                                lifetimes: trait_bound.lifetimes.clone(),
                                                path,
                                            }));
                                            modified = true;
                                        } else {
                                            new_bounds.push(TypeParamBound::Trait(trait_bound.clone()));
                                        }
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                }
                            }
                        }
                    }
                    if modified {
                        if let Some(where_clause) = &mut type_alias.generics.where_clause {
                            let mut new_predicates = Punctuated::new();
                            for predicate in &where_clause.predicates {
                                if let WherePredicate::Type(predicate) = predicate {
                                    new_predicates.push(WherePredicate::Type(PredicateType {
                                        lifetimes: predicate.lifetimes.clone(),
                                        bounded_ty: predicate.bounded_ty.clone(),
                                        colon_token: predicate.colon_token,
                                        bounds: new_bounds.clone(),
                                    }));
                                } else {
                                    new_predicates.push(predicate.clone());
                                }
                            }
                            where_clause.predicates = new_predicates;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets type alias definitions containing trait bounds. It modifies the first trait bound in the alias by either truncating its path (removing the last segment for multi-segment paths) or replacing it with an underscore placeholder for single-segment paths. This creates syntactically valid but semantically incomplete trait bounds, testing the compiler's diagnostic and resolution logic for malformed trait bounds in type aliases."
    }
}