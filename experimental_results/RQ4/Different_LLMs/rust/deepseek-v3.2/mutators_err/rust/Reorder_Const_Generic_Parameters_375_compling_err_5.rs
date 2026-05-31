use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, GenericParam::Const, GenericParam::Lifetime, GenericParam::Type, Item, ItemFn, ItemStruct, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, TypeImplTrait,
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

pub struct Reorder_Const_Generic_Parameters_375;

impl Mutator for Reorder_Const_Generic_Parameters_375 {
    fn name(&self) -> &str {
        "Reorder_Const_Generic_Parameters_375"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                let params = &mut generics.params;
                
                if params.len() < 2 {
                    continue;
                }
                
                let mut const_params = Vec::new();
                let mut other_params = Vec::new();
                let mut has_const = false;
                let mut has_other = false;
                
                for param in params.iter() {
                    match param {
                        GenericParam::Const(_) => {
                            const_params.push(param.clone());
                            has_const = true;
                        }
                        GenericParam::Type(_) | GenericParam::Lifetime(_) => {
                            other_params.push(param.clone());
                            has_other = true;
                        }
                    }
                }
                
                if !has_const || !has_other {
                    continue;
                }
                
                let mut first_is_const = false;
                if let Some(first_param) = params.first() {
                    if let GenericParam::Const(_) = first_param {
                        first_is_const = true;
                    }
                }
                
                if !first_is_const {
                    continue;
                }
                
                let mut new_params = Punctuated::new();
                
                for param in other_params {
                    new_params.push(param);
                }
                
                for param in const_params {
                    new_params.push(param);
                }
                
                if new_params.len() == params.len() {
                    *params = new_params;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with both const generic parameters and type/lifetime parameters, where const parameters appear first. It reorders the generic parameters to place const parameters after type/lifetime parameters while preserving all parameter identities, types, and default values. This transformation can trigger internal compiler errors in substitution logic and well-formedness checking, as parameter indices may become out of range, especially when default values are involved."
    }
}