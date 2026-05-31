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

pub struct Replace_Trait_Bound_With_Const_Generic_Array_379;

impl Mutator for Replace_Trait_Bound_With_Const_Generic_Array_379 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_With_Const_Generic_Array_379"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceTraitBoundVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets where clauses containing trait bounds with associated type projections. It replaces such bounds with const generic array bounds using the same associated type projection, e.g., `[u8; <T as Trait>::Assoc::CONST]: Sized`. This transformation stresses the compiler's normalization of associated types in const contexts and may trigger bugs related to const evaluation, trait resolution, and where-clause validation."
    }
}

struct ReplaceTraitBoundVisitor;

impl VisitMut for ReplaceTraitBoundVisitor {
    fn visit_where_clause_mut(&mut self, where_clause: &mut syn::WhereClause) {
        let mut new_predicates = Punctuated::new();
        for predicate in where_clause.predicates.iter() {
            match predicate {
                WherePredicate::Type(pred_type) => {
                    if let Type::Path(type_path) = &pred_type.bounded_ty {
                        if let Some(last_segment) = type_path.path.segments.last() {
                            if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                                for arg in &args.args {
                                    if let GenericArgument::AssocType(assoc) = arg {
                                        let new_predicate = create_const_generic_predicate(&assoc.ident, &type_path.path);
                                        new_predicates.push(new_predicate);
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                    if pred_type.bounds.iter().any(|bound| {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            trait_bound.path.segments.iter().any(|seg| {
                                if let PathArguments::AngleBracketed(args) = &seg.arguments {
                                    args.args.iter().any(|arg| matches!(arg, GenericArgument::AssocType(_)))
                                } else {
                                    false
                                }
                            })
                        } else {
                            false
                        }
                    }) {
                        let new_predicate = create_const_generic_predicate_from_bounds(&pred_type.bounds);
                        new_predicates.push(new_predicate);
                        continue;
                    }
                    new_predicates.push(predicate.clone());
                }
                _ => new_predicates.push(predicate.clone()),
            }
        }
        where_clause.predicates = new_predicates;
        self.visit_where_clause_mut(where_clause);
    }
}

fn create_const_generic_predicate(assoc_ident: &Ident, trait_path: &SynPath) -> WherePredicate {
    let const_array_type: Type = parse_quote! {
        [u8; <#trait_path as #trait_path>::#assoc_ident::CONST]
    };
    let sized_bound = TraitBound {
        paren_token: None,
        modifier: TraitBoundModifier::None,
        lifetimes: None,
        path: syn::Path::from(Ident::new("Sized", Span::call_site())),
    };
    let mut bounds = Punctuated::new();
    bounds.push(TypeParamBound::Trait(sized_bound));
    WherePredicate::Type(PredicateType {
        lifetimes: None,
        bounded_ty: const_array_type,
        colon_token: Default::default(),
        bounds,
    })
}

fn create_const_generic_predicate_from_bounds(bounds: &Punctuated<TypeParamBound, token::Plus>) -> WherePredicate {
    let mut rng = thread_rng();
    let trait_bound = bounds.iter().find_map(|b| {
        if let TypeParamBound::Trait(tb) = b {
            Some(tb)
        } else {
            None
        }
    });
    if let Some(tb) = trait_bound {
        let path = &tb.path;
        let const_array_type: Type = parse_quote! {
            [u8; <#path as #path>::Assoc::CONST]
        };
        let sized_bound = TraitBound {
            paren_token: None,
            modifier: TraitBoundModifier::None,
            lifetimes: None,
            path: syn::Path::from(Ident::new("Sized", Span::call_site())),
        };
        let mut bounds = Punctuated::new();
        bounds.push(TypeParamBound::Trait(sized_bound));
        WherePredicate::Type(PredicateType {
            lifetimes: None,
            bounded_ty: const_array_type,
            colon_token: Default::default(),
            bounds,
        })
    } else {
        let dummy_type: Type = parse_quote! { [u8; 0] };
        let sized_bound = TraitBound {
            paren_token: None,
            modifier: TraitBoundModifier::None,
            lifetimes: None,
            path: syn::Path::from(Ident::new("Sized", Span::call_site())),
        };
        let mut bounds = Punctuated::new();
        bounds.push(TypeParamBound::Trait(sized_bound));
        WherePredicate::Type(PredicateType {
            lifetimes: None,
            bounded_ty: dummy_type,
            colon_token: Default::default(),
            bounds,
        })
    }
}