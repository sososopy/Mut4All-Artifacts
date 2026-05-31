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

pub struct Add_Unused_Generic_Parameter_358;

impl Mutator for Add_Unused_Generic_Parameter_358 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Parameter_358"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let generics = &mut func.sig.generics;
                    let new_generic: GenericParam = parse_quote!(T);
                    generics.params.push(new_generic);

                    // Add a where clause that uses the new generic parameter in a complex way
                    let where_predicate: syn::WherePredicate = parse_quote!(T: std::fmt::Debug + std::marker::Send);
                    if let Some(where_clause) = &mut generics.where_clause {
                        where_clause.predicates.push(where_predicate);
                    } else {
                        generics.where_clause = Some(syn::WhereClause {
                            where_token: Default::default(),
                            predicates: {
                                let mut predicates = Punctuated::new();
                                predicates.push(where_predicate);
                                predicates
                            },
                        });
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            let generics = &mut func.sig.generics;
                            let new_generic: GenericParam = parse_quote!(U);
                            generics.params.push(new_generic);

                            // Add a where clause that uses the new generic parameter in a complex way
                            let where_predicate: syn::WherePredicate = parse_quote!(U: std::fmt::Display + std::marker::Sync);
                            if let Some(where_clause) = &mut generics.where_clause {
                                where_clause.predicates.push(where_predicate);
                            } else {
                                generics.where_clause = Some(syn::WhereClause {
                                    where_token: Default::default(),
                                    predicates: {
                                        let mut predicates = Punctuated::new();
                                        predicates.push(where_predicate);
                                        predicates
                                    },
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an unused generic parameter to function and method signatures, along with a complex where clause. This transformation aims to stress the compiler's generic parameter handling and type resolution capabilities by adding unnecessary complexity, potentially leading to ICEs or inference issues."
    }
}