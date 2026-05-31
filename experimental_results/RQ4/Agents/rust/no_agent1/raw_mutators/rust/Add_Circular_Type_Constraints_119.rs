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

pub struct Add_Circular_Type_Constraints_119;

impl Mutator for Add_Circular_Type_Constraints_119 {
    fn name(&self) -> &str {
        "Add_Circular_Type_Constraints_119"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                let type_param_a: GenericParam = parse_quote!(A);
                let type_param_b: GenericParam = parse_quote!(B);
                generics.params.push(type_param_a.clone());
                generics.params.push(type_param_b.clone());

                let mut where_clause = WhereClause {
                    where_token: Default::default(),
                    predicates: Punctuated::new(),
                };

                where_clause.predicates.push(syn::WherePredicate::Type(PredicateType {
                    lifetimes: None,
                    bounded_ty: syn::Type::Path(TypePath {
                        qself: None,
                        path: syn::Path::from(Ident::new("A", Span::call_site())),
                    }),
                    colon_token: Default::default(),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path::from(Ident::new("B", Span::call_site())),
                        }));
                        bounds
                    },
                }));

                where_clause.predicates.push(syn::WherePredicate::Type(PredicateType {
                    lifetimes: None,
                    bounded_ty: syn::Type::Path(TypePath {
                        qself: None,
                        path: syn::Path::from(Ident::new("B", Span::call_site())),
                    }),
                    colon_token: Default::default(),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path::from(Ident::new("A", Span::call_site())),
                        }));
                        bounds
                    },
                }));

                generics.where_clause = Some(where_clause);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces circular type constraints between two generic parameters `A` and `B`, where `A: B` and `B: A`. This creates a paradox in type resolution, challenging the compiler's ability to handle circular dependencies and potentially leading to ICEs or infinite loops in trait resolution."
    }
}