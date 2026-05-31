use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct HigherRankedAssoc_8;

impl Mutator for HigherRankedAssoc_8 {
    fn name(&self) -> &str {
        "HigherRankedAssoc_8"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                self.modify_trait_bounds(&mut func.sig.generics);
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        self.modify_trait_bounds(&mut method.sig.generics);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces higher-ranked lifetime bounds on associated types within trait bounds. It modifies existing trait bounds by wrapping associated types in `for<'a>` clauses or adds new associated types with such bounds. This increases the complexity of trait resolution and tests the compiler's handling of higher-ranked lifetimes and associated type constraints."
    }
}

impl HigherRankedAssoc_8 {
    fn modify_trait_bounds(&self, generics: &mut syn::Generics) {
        if let Some(where_clause) = &mut generics.where_clause {
            for predicate in &mut where_clause.predicates {
                if let syn::WherePredicate::Type(predicate_type) = predicate {
                    for bound in &mut predicate_type.bounds {
                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                            self.modify_trait_bound(trait_bound);
                        }
                    }
                }
            }
        }
    }

    fn modify_trait_bound(&self, trait_bound: &mut TraitBound) {
        let path = &mut trait_bound.path;
        let last_segment = path.segments.last_mut().unwrap();

        let args = match &mut last_segment.arguments {
            PathArguments::AngleBracketed(args) => args,
            PathArguments::Parenthesized(_) => return,
            PathArguments::None => {
                let new_args = AngleBracketedGenericArguments {
                    colon2_token: None,
                    lt_token: token::Lt::default(),
                    args: Punctuated::new(),
                    gt_token: token::Gt::default(),
                };
                last_segment.arguments = PathArguments::AngleBracketed(new_args);
                last_segment.arguments.as_angle_bracketed_mut().unwrap()
            }
        };

        let has_assoc_type = args.args.iter().any(|arg| matches!(arg, GenericArgument::AssocType(_)));

        if has_assoc_type {
            for arg in &mut args.args {
                if let GenericArgument::AssocType(assoc_type) = arg {
                    let new_value = parse_quote!(for<'a> 'a);
                    *assoc_type.ty = Box::new(new_value);
                }
            }
        } else {
            let new_assoc_type = AssocType {
                ident: Ident::new("AssocType", Span::call_site()),
                colon2_token: None,
                eq_token: Some(Default::default()),
                ty: Box::new(parse_quote!(for<'a> 'a)),
            };
            args.args.push(GenericArgument::AssocType(new_assoc_type));
        }
    }
}