use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, visit::Visit, visit_mut::VisitMut, *,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, token::Comma, token::{Paren, Plus},
};

use crate::mutator::Mutator;

pub struct Introduce_Variadic_Args_24;

impl Mutator for Introduce_Variadic_Args_24 {
    fn name(&self) -> &str {
        "Introduce_Variadic_Args_24"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                // Add a variadic argument to the function signature
                func.sig.inputs.push(parse_quote!(args: ...));

                // Modify the where clause to include a variadic bound
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates.push(parse_quote!(args: std::fmt::Debug));
                } else {
                    func.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(parse_quote!(args: std::fmt::Debug));
                            predicates
                        },
                    });
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        ""
    }
}