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

pub struct Modify_Const_Generic_Parameter_404;

impl Mutator for Modify_Const_Generic_Parameter_404 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameter_404"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(const_generic) = func.sig.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        Some(const_param)
                    } else {
                        None
                    }
                }) {
                    const_generic.ty = parse_quote!(bool);
                    
                    if let Some(where_clause) = &mut func.sig.generics.where_clause {
                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(pred_type) = predicate {
                                if let syn::Type::Path(type_path) = &pred_type.bounded_ty {
                                    if type_path.path.is_ident(&const_generic.ident) {
                                        pred_type.bounded_ty = parse_quote!(bool);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function definitions with const-generic parameters, changing one of them from its original type to `bool`. This alteration is reflected in the function's where clause, potentially leading to type mismatches and testing the compiler's handling of const-generic parameters and type constraints."
    }
}