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

pub struct Introduce_Late_Bound_Const_In_Invalid_Context_109;

impl Mutator for Introduce_Late_Bound_Const_In_Invalid_Context_109 {
    fn name(&self) -> &str {
        "Introduce_Late_Bound_Const_In_Invalid_Context_109"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                // Collect const generic parameters
                let mut const_params: Vec<_> = generics.params.iter().filter_map(|param| {
                    if let GenericParam::Const(param) = param {
                        Some(param.ident.clone())
                    } else {
                        None
                    }
                }).collect();
                if const_params.is_empty() {
                    continue;
                }
                // Check if there is a where clause
                if let Some(where_clause) = &mut generics.where_clause {
                    // Iterate over the predicates
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(pred) = predicate {
                            // Check if the bounded_ty is a Type::Array
                            if let Type::Array(array) = pred.bounded_ty.as_ref() {
                                // Check if the array's length is a reference to a const param
                                if let Type::Path(path) = array.len.as_ref() {
                                    let ident = path.path.get_ident();
                                    if let Some(ident) = ident {
                                        if const_params.contains(&ident) {
                                            // Find the corresponding const param in the function's generics
                                            let mut const_param_to_remove = None;
                                            let mut param_index = None;
                                            for (i, param) in generics.params.iter().enumerate() {
                                                if let GenericParam::Const(c) = param {
                                                    if c.ident == *ident {
                                                        const_param_to_remove = Some(c.clone());
                                                        param_index = Some(i);
                                                        break;
                                                    }
                                                }
                                            }
                                            if let (Some(c), Some(i)) = (const_param_to_remove, param_index) {
                                                // Remove the const param from the function's generics
                                                generics.params = generics.params.into_iter().enumerate().filter(|(idx, _)| *idx != i).collect();
                                                // Create the for binder's generics
                                                let for_binder_generics = Punctuated::from_iter(vec![GenericParam::Const(c.clone())]);
                                                // Create the new WherePredicate::For
                                                let new_predicate = syn::WherePredicate::For(syn::WhereForPredicate {
                                                    for_token: syn::token::For::default(),
                                                    generic_params: for_binder_generics,
                                                    colon_token: Default::default(),
                                                    predicate: Box::new(syn::WherePredicate::Type(pred.clone())),
                                                });
                                                // Replace the current predicate with the new one
                                                *predicate = new_predicate;
                                                // Break after modifying the first found predicate
                                                break;
                                            }
                                        }
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
        ""
    }
}