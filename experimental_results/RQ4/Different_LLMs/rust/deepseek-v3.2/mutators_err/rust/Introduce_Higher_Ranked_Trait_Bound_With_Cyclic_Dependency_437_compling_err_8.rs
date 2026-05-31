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

pub struct Introduce_Higher_Ranked_Trait_Bound_With_Cyclic_Dependency_437;

impl Mutator for Introduce_Higher_Ranked_Trait_Bound_With_Cyclic_Dependency_437 {
    fn name(&self) -> &str {
        "Introduce_Higher_Ranked_Trait_Bound_With_Cyclic_Dependency_437"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                
                let mut trait_bounds = Vec::new();
                let mut existing_generics = Vec::new();
                
                if let Some(where_clause) = &item_fn.sig.generics.where_clause {
                    for predicate in &where_clause.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            if let Some(TypeParamBound::Trait(trait_bound)) = pred_type.bounds.first() {
                                trait_bounds.push(trait_bound.clone());
                            }
                        }
                    }
                }
                
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        existing_generics.push(type_param.ident.clone());
                    }
                }
                
                if trait_bounds.is_empty() || existing_generics.is_empty() {
                    continue;
                }
                
                let mut rng = thread_rng();
                let selected_trait = trait_bounds.choose(&mut rng).unwrap();
                let selected_generic = existing_generics.choose(&mut rng).unwrap();
                
                let new_generic_ident = Ident::new("HRTB_T", Span::call_site());
                item_fn.sig.generics.params.push(parse_quote!(#new_generic_ident));
                
                let mut new_where_clause = item_fn.sig.generics.where_clause.take().unwrap_or_else(|| {
                    WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::new(),
                    }
                });
                
                let hrtb_predicate: WherePredicate = parse_quote! {
                    #selected_generic: for<'a> #selected_trait<Assoc<'a> = #new_generic_ident::Assoc<'a>>
                };
                
                let cyclic_predicate: WherePredicate = parse_quote! {
                    #new_generic_ident: for<'a> #selected_trait<Assoc<'a> = #selected_generic::Assoc<'a>>
                };
                
                new_where_clause.predicates.push(hrtb_predicate);
                new_where_clause.predicates.push(cyclic_predicate);
                
                item_fn.sig.generics.where_clause = Some(new_where_clause);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces higher-ranked trait bounds (HRTB) with cyclic dependencies between generic parameters. It targets functions with existing trait bounds in where-clauses, adds a new type parameter, and creates mutually recursive HRTB constraints using for<T> syntax. This transformation stresses the compiler's trait solving logic, particularly for generic associated types under non_lifetime_binders, potentially causing infinite recursion or complex normalization failures."
    }
}