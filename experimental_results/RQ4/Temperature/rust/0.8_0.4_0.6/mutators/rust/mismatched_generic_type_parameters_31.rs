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

pub struct Mismatched_Generic_Type_Parameters_31;

impl Mutator for Mismatched_Generic_Type_Parameters_31 {
    fn name(&self) -> &str {
        "Mismatched_Generic_Type_Parameters_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut rng = thread_rng();
                if func.sig.generics.params.len() > 1 {
                    let mut param_names: Vec<_> = func.sig.generics.params.iter().collect();
                    param_names.shuffle(&mut rng);
                    let first_param = param_names[0].clone();
                    let second_param = param_names[1].clone();
                    
                    if let GenericParam::Type(first_type_param) = first_param {
                        if let GenericParam::Type(second_type_param) = second_param {
                            let new_ident = Ident::new(&format!("{}_", second_type_param.ident), second_type_param.ident.span());
                            let new_type_param = GenericParam::Type(TypeParam {
                                ident: new_ident,
                                ..second_type_param.clone()
                            });
                            
                            func.sig.generics.params = func.sig.generics.params.iter().map(|param| {
                                if let GenericParam::Type(type_param) = param {
                                    if type_param.ident == second_type_param.ident {
                                        return new_type_param.clone();
                                    }
                                }
                                param.clone()
                            }).collect();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function signatures with multiple generic type parameters. It randomly selects two type parameters and introduces a mismatch by renaming one of them, potentially exposing type handling inconsistencies in the compiler. This mutation aims to test the compiler's ability to detect and handle mismatched generic type parameters."
    }
}