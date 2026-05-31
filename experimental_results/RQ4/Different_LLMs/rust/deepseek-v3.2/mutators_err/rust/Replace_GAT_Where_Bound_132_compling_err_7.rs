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

pub struct Replace_GAT_Where_Bound_132;

impl Mutator for Replace_GAT_Where_Bound_132 {
    fn name(&self) -> &str {
        "Replace_GAT_Where_Bound_132"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut has_lifetime_param = false;
                let mut existing_lifetime = None;
                for param in &trait_item.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        has_lifetime_param = true;
                        existing_lifetime = Some(lifetime_param.lifetime.clone());
                        break;
                    }
                }
                let target_lifetime = if has_lifetime_param {
                    existing_lifetime.unwrap()
                } else {
                    let new_lifetime = Lifetime::new("'b", Span::call_site());
                    trait_item.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: new_lifetime.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    new_lifetime
                };
                for item in &mut trait_item.items {
                    if let TraitItem::Type(type_item) = item {
                        if let Some(where_clause) = &mut type_item.generics.where_clause {
                            let mut predicates_to_add = Vec::new();
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                        if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == "Self" {
                                            for bound in &mut predicate_type.bounds {
                                                if let TypeParamBound::Lifetime(lifetime_bound) = bound {
                                                    *lifetime_bound = target_lifetime.clone();
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if rng.gen_bool(0.5) {
                                predicates_to_add.push(WherePredicate::Lifetime(PredicateLifetime {
                                    lifetime: target_lifetime.clone(),
                                    colon_token: Default::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(Lifetime::new("'a", Span::call_site()));
                                        bounds
                                    },
                                }));
                            }
                            where_clause.predicates.extend(predicates_to_add);
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                let mut has_lifetime_param = false;
                let mut existing_lifetime = None;
                for param in &impl_item.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        has_lifetime_param = true;
                        existing_lifetime = Some(lifetime_param.lifetime.clone());
                        break;
                    }
                }
                let target_lifetime = if has_lifetime_param {
                    existing_lifetime.unwrap()
                } else {
                    let new_lifetime = Lifetime::new("'b", Span::call_site());
                    impl_item.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: new_lifetime.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    new_lifetime
                };
                for item in &mut impl_item.items {
                    if let ImplItem::Type(type_item) = item {
                        if let Some(where_clause) = &mut type_item.generics.where_clause {
                            let mut predicates_to_add = Vec::new();
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                        if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == "Self" {
                                            for bound in &mut predicate_type.bounds {
                                                if let TypeParamBound::Lifetime(lifetime_bound) = bound {
                                                    *lifetime_bound = target_lifetime.clone();
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if rng.gen_bool(0.5) {
                                predicates_to_add.push(WherePredicate::Lifetime(PredicateLifetime {
                                    lifetime: target_lifetime.clone(),
                                    colon_token: Default::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(Lifetime::new("'a", Span::call_site()));
                                        bounds
                                    },
                                }));
                            }
                            where_clause.predicates.extend(predicates_to_add);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic associated type (GAT) definitions within trait or impl blocks that have a where clause containing a bound relating the GAT's lifetime parameter to the Self type. When a GAT definition includes a where clause of the form `where Self: 'a`, it replaces this bound with a bound that uses a different lifetime. Specifically, it changes `where Self: 'a` to `where Self: 'b`, where `'b` is a new lifetime parameter introduced on the trait or impl if not already present. If the trait or impl already has a lifetime parameter, it uses the existing one. In impl blocks, the same change is applied. Additionally, it may add a lifetime bound `'b: 'a` to maintain coherence. This transformation creates a mismatch or novel relation between the GAT's lifetime parameter and the bound on Self, potentially exposing compiler bugs in region checking for GATs."
    }
}