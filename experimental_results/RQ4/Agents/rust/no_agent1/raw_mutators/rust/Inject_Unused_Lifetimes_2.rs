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

pub struct Inject_Unused_Lifetimes_2;

impl Mutator for Inject_Unused_Lifetimes_2 {
    fn name(&self) -> &str {
        "Inject_Unused_Lifetimes_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_a = Lifetime::new("'a", Span::call_site());
                let lifetime_b = Lifetime::new("'b", Span::call_site());
                func.sig.generics.params.push(GenericParam::Lifetime(LifetimeDef::new(lifetime_a.clone())));
                func.sig.generics.params.push(GenericParam::Lifetime(LifetimeDef::new(lifetime_b.clone())));
                
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates.push(parse_quote!('a: 'b));
                } else {
                    func.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(parse_quote!('a: 'b));
                            predicates
                        },
                    });
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetime_a = Lifetime::new("'a", Span::call_site());
                        let lifetime_b = Lifetime::new("'b", Span::call_site());
                        func.sig.generics.params.push(GenericParam::Lifetime(LifetimeDef::new(lifetime_a.clone())));
                        func.sig.generics.params.push(GenericParam::Lifetime(LifetimeDef::new(lifetime_b.clone())));
                        
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            where_clause.predicates.push(parse_quote!('a: 'b));
                        } else {
                            func.sig.generics.where_clause = Some(WhereClause {
                                where_token: Default::default(),
                                predicates: {
                                    let mut predicates = Punctuated::new();
                                    predicates.push(parse_quote!('a: 'b));
                                    predicates
                                },
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces unused lifetimes 'a and 'b into function and method signatures, along with a where clause 'a: 'b. This transformation aims to stress the compiler's lifetime resolution system by adding redundant and potentially conflicting lifetime constraints, which may lead to ICEs or unexpected behavior in complex lifetime scenarios."
    }
}