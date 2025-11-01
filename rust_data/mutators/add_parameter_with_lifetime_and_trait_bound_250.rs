use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Add_Parameter_With_Lifetime_And_Trait_Bound_250;

impl Mutator for Add_Parameter_With_Lifetime_And_Trait_Bound_250 {
    fn name(&self) -> &str {
        "Add_Parameter_With_Lifetime_And_Trait_Bound_250"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item{
                let mut generics = func.sig.generics.clone();
                if generics.params.is_empty() {
                    generics
                        .params
                        .push(syn::GenericParam::Type(syn::TypeParam {
                            attrs: Vec::new(),
                            ident: Ident::new("T_1", func.span()),
                            colon_token: None,
                            bounds: syn::punctuated::Punctuated::new(),
                            eq_token: None,
                            default: None,
                        }));
                }
                let where_clause = generics.where_clause.get_or_insert_with(|| WhereClause {
                    where_token: token::Where::default(),
                    predicates: syn::punctuated::Punctuated::new(),
                });
                where_clause
                    .predicates
                    .push(syn::WherePredicate::Type(syn::PredicateType {
                        lifetimes: Some(syn::BoundLifetimes {
                            for_token: token::For::default(),
                            lt_token: token::Lt::default(),
                            lifetimes: syn::punctuated::Punctuated::from_iter(vec![
                                syn::GenericParam::Lifetime(syn::LifetimeParam {
                                    attrs: Vec::new(),
                                    lifetime: Lifetime::new("'a_1", func.span()),
                                    colon_token: None,
                                    bounds: syn::punctuated::Punctuated::new(),
                                }),
                            ]),
                            gt_token: token::Gt::default(),
                        }),
                        bounded_ty: Type::Path(syn::TypePath {
                            qself: None,
                            path: syn::Path::from(Ident::new("T_1", func.span())),
                        }),
                        colon_token: token::Colon::default(),
                        bounds: syn::punctuated::Punctuated::from_iter(vec![syn::TypeParamBound::Trait(
                            syn::TraitBound {
                                paren_token: None,
                                modifier: syn::TraitBoundModifier::None,
                                lifetimes: None,
                                path: syn::Path::from(Ident::new("Sized", func.span())),
                            },
                        )]),
                    }));
                func.sig.generics = generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets all function definitions, including free functions and inherent methods. It adds a new generic type parameter (e.g., T0) and inserts a where clause constraint like T0: 'a + Sized, where 'a is a synthetic lifetime. This mutation tests the compiler’s handling of generic bounds, lifetime resolution, and trait constraints. It may expose issues in how the compiler processes extra, complex type relationships in functions."
    }
}