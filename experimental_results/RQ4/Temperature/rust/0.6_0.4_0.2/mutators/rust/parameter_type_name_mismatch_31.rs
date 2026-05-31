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

pub struct Parameter_Type_Name_Mismatch_31;

impl Mutator for Parameter_Type_Name_Mismatch_31 {
    fn name(&self) -> &str {
        "Parameter_Type_Name_Mismatch_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if !func.sig.generics.params.is_empty() {
                    let mut rng = thread_rng();
                    let type_params: Vec<_> = func.sig.generics.params.iter().filter_map(|param| {
                        if let syn::GenericParam::Type(type_param) = param {
                            Some(type_param.ident.clone())
                        } else {
                            None
                        }
                    }).collect();

                    if !type_params.is_empty() {
                        let chosen_param = type_params.choose(&mut rng).unwrap();
                        for stmt in &mut func.block.stmts {
                            if let syn::Stmt::Local(local) = stmt {
                                if let syn::Pat::Type(PatType { ty, .. }) = &mut local.pat {
                                    if let syn::Type::Path(TypePath { path, .. }) = &**ty {
                                        if path.segments.len() == 1 && path.segments[0].ident == *chosen_param {
                                            let new_ident = Ident::new("U", Span::call_site());
                                            *ty = Box::new(syn::Type::Path(TypePath {
                                                qself: None,
                                                path: syn::Path::from(new_ident),
                                            }));
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
        "This mutation operator targets generic functions and introduces a type parameter name mismatch by renaming one of the type parameters used in the function body to a different identifier. This transformation aims to test the compiler's handling of type parameter mismatches and can help uncover issues related to type parameter resolution in the Rust type system."
    }
}