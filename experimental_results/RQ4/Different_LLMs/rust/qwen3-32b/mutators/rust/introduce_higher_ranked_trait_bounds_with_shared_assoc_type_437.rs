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

pub struct Introduce_Higher_Ranked_Trait_Bounds_With_Shared_Assoc_Type_437;

impl Mutator for Introduce_Higher_Ranked_Trait_Bounds_With_Shared_Assoc_Type_437 {
    fn name(&self) -> &str {
        "Introduce_Higher_Ranked_Trait_Bounds_With_Shared_Assoc_Type_437"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if func.sig.generics.params.len() < 2 {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    let first_param = match &func.sig.generics.params[0] {
                        GenericParam::Type(ty) => &ty.ident,
                        _ => continue,
                    };
                    let second_param = match &func.sig.generics.params[1] {
                        GenericParam::Type(ty) => &ty.ident,
                        _ => continue,
                    };
                    let first_pred = parse_quote!(#first_param: for<'a> Trait<Assoc = Bar<'a>>);
                    let second_pred = parse_quote!(#second_param: for<'a> Trait<Assoc = #first_param::Bar<'a>>);
                    where_clause.predicates.push(first_pred);
                    where_clause.predicates.push(second_pred);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}