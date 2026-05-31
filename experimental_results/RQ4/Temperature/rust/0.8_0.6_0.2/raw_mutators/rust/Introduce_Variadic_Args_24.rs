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
                func.sig.inputs.push(parse_quote!(...));

                // Modify the where clause to include a variadic bound
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates.push(parse_quote!(...: SomeTrait));
                } else {
                    func.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(parse_quote!(...: SomeTrait));
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