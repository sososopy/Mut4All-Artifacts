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

pub struct Modify_Trait_Bounds_In_Generic_Implementations_265;

impl Mutator for Modify_Trait_Bounds_In_Generic_Implementations_265 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_In_Generic_Implementations_265"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_some() {
                    let mut has_generic = false;
                    for param in &item_impl.generics.params {
                        if matches!(param, GenericParam::Type(_)) {
                            has_generic = true;
                            break;
                        }
                    }
                    if has_generic {
                        let mut has_deserialize_owned_bound = false;
                        if let Some(where_clause) = &item_impl.generics.where_clause {
                            for predicate in &where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    for bound in &predicate_type.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if trait_bound.path.is_ident("DeserializeOwned") {
                                                has_deserialize_owned_bound = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if has_deserialize_owned_bound {
                                    break;
                                }
                            }
                        }
                        if has_deserialize_owned_bound {
                            if let Some(where_clause) = &mut item_impl.generics.where_clause {
                                for predicate in &mut where_clause.predicates {
                                    if let WherePredicate::Type(predicate_type) = predicate {
                                        for bound in &mut predicate_type.bounds {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                if trait_bound.path.is_ident("DeserializeOwned") {
                                                    trait_bound.path.segments.push_punct(
                                                        syn::token::PathSep::default(),
                                                    );
                                                    trait_bound.path.segments.push_value(
                                                        PathSegment {
                                                            ident: Ident::new(
                                                                "Clone",
                                                                Span::call_site(),
                                                            ),
                                                            arguments:
                                                                PathArguments::None,
                                                        },
                                                    );
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
        "This mutator targets generic implementations with trait bounds, specifically looking for `DeserializeOwned` constraints. It modifies these bounds by adding a `Clone` constraint, transforming `T: DeserializeOwned` into `T: DeserializeOwned + Clone`. This alteration tests the compiler's handling of additional trait constraints in generic contexts, which can reveal issues in trait resolution and type checking, especially in more complex generic and trait scenarios."
    }
}