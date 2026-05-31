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

pub struct Add_Higher_Ranked_Associated_Type_Bound_296;

impl Mutator for Add_Higher_Ranked_Associated_Type_Bound_296 {
    fn name(&self) -> &str {
        "Add_Higher_Ranked_Associated_Type_Bound_296"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits_with_lifetime_param = Vec::new();
        let mut visitor = TraitCollector {
            traits: &mut traits_with_lifetime_param,
        };
        visitor.visit_file(file);

        if traits_with_lifetime_param.is_empty() {
            return;
        }

        let mut visitor = WhereClauseMutator {
            traits: &traits_with_lifetime_param,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies where clauses containing associated type bounds (e.g., Trait<Assoc: Bound>) and transforms them by replacing the bound with a higher-ranked trait bound (for<'a> TraitL<'a>). It first collects all traits in the seed program that take lifetime parameters, then mutates eligible where clauses in functions, impls, and trait definitions. This creates complex nested bounds that stress the compiler's bound variable resolution and higher-ranked trait bound handling, potentially exposing issues similar to those in the bug report."
    }
}

struct TraitCollector<'a> {
    traits: &'a mut Vec<syn::Path>,
}

impl<'a> Visit<'a> for TraitCollector<'a> {
    fn visit_item_trait(&mut self, node: &'a syn::ItemTrait) {
        let mut has_lifetime_param = false;
        for param in &node.generics.params {
            if let GenericParam::Lifetime(_) = param {
                has_lifetime_param = true;
                break;
            }
        }
        if has_lifetime_param {
            let path = syn::Path {
                leading_colon: None,
                segments: {
                    let mut segments = Punctuated::new();
                    segments.push(syn::PathSegment {
                        ident: node.ident.clone(),
                        arguments: syn::PathArguments::None,
                    });
                    segments
                },
            };
            self.traits.push(path);
        }
        syn::visit::visit_item_trait(self, node);
    }
}

struct WhereClauseMutator<'a> {
    traits: &'a Vec<syn::Path>,
}

impl<'a> VisitMut for WhereClauseMutator<'a> {
    fn visit_where_clause_mut(&mut self, node: &mut syn::WhereClause) {
        for predicate in &mut node.predicates {
            if let syn::WherePredicate::Type(pred_type) = predicate {
                self.mutate_type_predicate(pred_type);
            }
        }
        syn::visit_mut::visit_where_clause_mut(self, node);
    }

    fn visit_item_fn_mut(&mut self, node: &mut syn::ItemFn) {
        if let Some(ref mut where_clause) = node.sig.generics.where_clause {
            self.visit_where_clause_mut(where_clause);
        }
        syn::visit_mut::visit_item_fn_mut(self, node);
    }

    fn visit_item_impl_mut(&mut self, node: &mut syn::ItemImpl) {
        if let Some(ref mut where_clause) = node.generics.where_clause {
            self.visit_where_clause_mut(where_clause);
        }
        syn::visit_mut::visit_item_impl_mut(self, node);
    }

    fn visit_item_trait_mut(&mut self, node: &mut syn::ItemTrait) {
        if let Some(ref mut where_clause) = node.generics.where_clause {
            self.visit_where_clause_mut(where_clause);
        }
        syn::visit_mut::visit_item_trait_mut(self, node);
    }
}

impl<'a> WhereClauseMutator<'a> {
    fn mutate_type_predicate(&self, pred_type: &mut syn::PredicateType) {
        if let syn::Type::Path(type_path) = &mut pred_type.bounded_ty {
            self.mutate_type_path(type_path);
        }
    }

    fn mutate_type_path(&self, type_path: &mut syn::TypePath) {
        if let Some(last_segment) = type_path.path.segments.last_mut() {
            if let syn::PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                for arg in &mut args.args {
                    if let syn::GenericArgument::AssocType(binding) = arg {
                        if let Some(trait_path) = self.traits.choose(&mut thread_rng()) {
                            let new_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                                paren_token: None,
                                modifier: syn::TraitBoundModifier::None,
                                lifetimes: Some(syn::BoundLifetimes {
                                    lifetimes: {
                                        let mut lifetimes = Punctuated::new();
                                        lifetimes.push(syn::GenericParam::Lifetime(syn::LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: syn::Lifetime::new("'a", Span::call_site()),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                        }));
                                        lifetimes
                                    },
                                    for_token: token::For::default(),
                                    gt_token: token::Gt::default(),
                                    lt_token: token::Lt::default(),
                                }),
                                path: trait_path.clone(),
                            });
                            binding.eq_token = syn::token::Eq::default();
                            binding.ty = syn::Type::Verbatim(quote! {});
                        }
                    }
                }
            }
        }
    }
}