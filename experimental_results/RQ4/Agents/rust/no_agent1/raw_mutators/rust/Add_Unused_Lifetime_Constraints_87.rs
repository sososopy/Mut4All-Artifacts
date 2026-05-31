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

pub struct Add_Unused_Lifetime_Constraints_87;

impl Mutator for Add_Unused_Lifetime_Constraints_87 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Constraints_87"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime = Lifetime::new("'a", Span::call_site());
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                func.sig.generics.where_clause.get_or_insert_with(|| WhereClause {
                    where_token: Default::default(),
                    predicates: Punctuated::new(),
                }).predicates.push(WherePredicate::Lifetime(PredicateLifetime {
                    lifetime: lifetime.clone(),
                    colon_token: Default::default(),
                    bounds: Punctuated::from_iter(vec![Lifetime::new("'static", Span::call_site())]),
                }));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let lifetime = Lifetime::new("'b", Span::call_site());
                        let lifetime_param = LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: lifetime.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        };
                        func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                        func.sig.generics.where_clause.get_or_insert_with(|| WhereClause {
                            where_token: Default::default(),
                            predicates: Punctuated::new(),
                        }).predicates.push(WherePredicate::Lifetime(PredicateLifetime {
                            lifetime: lifetime.clone(),
                            colon_token: Default::default(),
                            bounds: Punctuated::from_iter(vec![Lifetime::new("'static", Span::call_site())]),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused lifetime parameters and constraints to function and method signatures. By introducing lifetimes that are not utilized in the function body or parameters, it challenges the compiler's lifetime resolution and inference mechanisms, potentially leading to ICEs or unexpected behavior in the presence of redundant or conflicting lifetime constraints."
    }
}