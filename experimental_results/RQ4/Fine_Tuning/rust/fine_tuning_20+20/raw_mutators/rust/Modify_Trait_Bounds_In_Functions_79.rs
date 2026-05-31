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

pub struct Modify_Trait_Bounds_In_Functions_79;

impl Mutator for Modify_Trait_Bounds_In_Functions_79 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_In_Functions_79"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_trait_bound = false;
                if let Some(where_clause) = &item_fn.sig.generics.where_clause {
                    for predicate in &where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    has_trait_bound = true;
                                    break;
                                }
                            }
                        }
                        if has_trait_bound {
                            break;
                        }
                    }
                }
                if has_trait_bound {
                    let new_where_clause: syn::WhereClause = parse_quote! {
                        where T: Sized
                    };
                    item_fn.sig.generics.where_clause = Some(new_where_clause);
                    let new_param: syn::FnArg = parse_quote! { _phantom: std::marker::PhantomData<T> };
                    item_fn.sig.inputs.push(new_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify functions with trait bounds in their where clause, replace the where clause with a trivial bound like `T: Sized`, and add a new function parameter using `std::marker::PhantomData<T>` to ensure T is used in a way that requires the original trait bound."
    }
}