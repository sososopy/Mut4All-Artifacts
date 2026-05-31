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

pub struct Add_Array_Length_Const_Bound_In_GAT_Impl_WhereClause_139;

impl Mutator for Add_Array_Length_Const_Bound_In_GAT_Impl_WhereClause_139 {
    fn name(&self) -> &str {
        "Add_Array_Length_Const_Bound_In_GAT_Impl_WhereClause_139"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                for ty_param in item_struct.generics.type_params() {
                    for bound in &ty_param.bounds {
                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                            let ty_param_ident = &ty_param.ident;
                            let trait_path = &trait_bound.path;
                            let where_predicate = parse_quote!( [(); <#ty_param_ident as #trait_path>::N]: );
                            if let Some(where_clause) = &mut item_struct.generics.where_clause {
                                where_clause.predicates.push(where_predicate);
                            } else {
                                let new_where_clause = parse_quote!( where [(); <#ty_param_ident as #trait_path>::N]: );
                                item_struct.generics.where_clause = Some(new_where_clause);
                            }
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