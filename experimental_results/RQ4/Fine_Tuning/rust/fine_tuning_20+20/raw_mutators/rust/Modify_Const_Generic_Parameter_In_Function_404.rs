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

pub struct Modify_Const_Generic_Parameter_In_Function_404;

impl Mutator for Modify_Const_Generic_Parameter_In_Function_404 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameter_In_Function_404"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut const_generic_indices = Vec::new();
                for (index, param) in func.sig.generics.params.iter().enumerate() {
                    if matches!(param, syn::GenericParam::Const(_)) {
                        const_generic_indices.push(index);
                    }
                }
                if const_generic_indices.is_empty() {
                    continue;
                }
                let mut rng = thread_rng();
                let target_index = const_generic_indices
                    .choose(&mut rng)
                    .copied()
                    .unwrap_or(0);
                let mut target_type = None;
                if let Some(where_clause) = &func.sig.generics.where_clause {
                    for predicate in &where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if segment.ident == "usize" || segment.ident == "isize" {
                                        target_type = Some(segment.ident.clone());
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if let Some(target_type) = target_type {
                    let new_param: syn::GenericParam = if target_type == "usize" {
                        parse_quote!(const _: bool)
                    } else {
                        parse_quote!(const _: usize)
                    };
                    func.sig.generics.params[target_index] = new_param;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function definitions that use const-generic parameters. It identifies a const-generic parameter in the function signature and modifies it to a different type or value that is incompatible with its original usage in the function's context. By altering the const-generic parameter, it stresses the compiler's ability to handle const-generic type checking and constant evaluation, potentially triggering bugs or ICEs related to const-generic inference and substitution."
    }
}