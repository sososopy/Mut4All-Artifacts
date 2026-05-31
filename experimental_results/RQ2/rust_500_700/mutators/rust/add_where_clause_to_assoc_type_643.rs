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

pub struct Add_Where_Clause_To_Assoc_Type_643;

impl Mutator for Add_Where_Clause_To_Assoc_Type_643 {
    fn name(&self) -> &str {
        "Add_Where_Clause_To_Assoc_Type_643"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        if !type_item.bounds.is_empty() {
                            type_item.generics.where_clause = Some(syn::WhereClause {
                                where_token: token::Where {
                                    span: Span::call_site(),
                                },
                                predicates: {
                                    let mut predicates = Punctuated::new();
                                    predicates.push(syn::WherePredicate::Type(syn::PredicateType {
                                        lifetimes: None,
                                        bounded_ty: syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: syn::Path::from(Ident::new("Self", Span::call_site())),
                                        }),
                                        colon_token: token::Colon {
                                            spans: [Span::call_site()],
                                        },
                                        bounds: {
                                            let mut bounds = Punctuated::new();
                                            bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                                paren_token: None,
                                                modifier: syn::TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: syn::Path::from(Ident::new("AnotherTrait", Span::call_site())),
                                            }));
                                            bounds
                                        },
                                    }));
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
        ""
    }
}