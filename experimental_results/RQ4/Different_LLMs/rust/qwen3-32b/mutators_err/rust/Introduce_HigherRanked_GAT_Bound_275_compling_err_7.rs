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

pub struct Introduce_HigherRanked_GAT_Bound_275;

impl Mutator for Introduce_HigherRanked_GAT_Bound_275 {
    fn name(&self) -> &str {
        "Introduce_HigherRanked_GAT_Bound_275"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(trait_type) = item {
                        if let Some(lifetime_def) = trait_type.generics.lifetimes().next() {
                            if let Some((_, ty)) = &trait_type.default {
                                if let Type::TraitObject(trait_obj) = ty {
                                    let mut has_gat_bound = false;
                                    let mut first_trait_bound_path = None;
                                    for bound in &trait_obj.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(path_segment) = trait_bound.path.segments.iter().next() {
                                                if path_segment.ident == "Iterator" {
                                                    has_gat_bound = true;
                                                    first_trait_bound_path = Some(trait_bound.path.clone());
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    if has_gat_bound {
                                        let new_bound = TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::For,
                                            lifetimes: Some(BoundLifetimes {
                                                for_token: token::For::default(),
                                                lt_token: token::Lt::default(),
                                                lifetimes: {
                                                    let mut lifetimes = Punctuated::new();
                                                    lifetimes.push(syn::GenericParam::Lifetime(LifetimeParam {
                                                        attrs: vec![],
                                                        lifetime: Lifetime::new("'b", Span::call_site()),
                                                        colon_token: Default::default(),
                                                        bounds: Punctuated::new(),
                                                    }));
                                                    lifetimes
                                                },
                                                gt_token: token::Gt::default(),
                                            }),
                                            path: first_trait_bound_path.unwrap(),
                                        };
                                        let new_bounds = {
                                            let mut bounds = Punctuated::new();
                                            bounds.push(syn::TypeParamBound::Trait(new_bound));
                                            bounds
                                        };
                                        trait_type.generics.where_clause = Some(WhereClause {
                                            where_token: Default::default(),
                                            predicates: {
                                                let mut preds = Punctuated::new();
                                                preds.push(WherePredicate::Type(PredicateType {
                                                    lifetimes: None,
                                                    bounded_ty: {
                                                        let mut type_path = TypePath {
                                                            qself: None,
                                                            path: trait_type.ident.clone().into(),
                                                        };
                                                        Type::Path(type_path)
                                                    },
                                                    colon_token: Default::default(),
                                                    bounds: new_bounds,
                                                }));
                                                preds
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
    fn chain_of_thought(&self) -> &str {
        ""
    }
}