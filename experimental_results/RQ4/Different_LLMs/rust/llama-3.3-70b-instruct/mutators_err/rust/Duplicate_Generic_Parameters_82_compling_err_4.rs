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

pub struct Duplicate_Generic_Parameters_82;

impl Mutator for Duplicate_Generic_Parameters_82 {
    fn name(&self) -> &str {
        "Duplicate_Generic_Parameters_82"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    let generics_mut = generics;
                    if !generics_mut.params.is_empty() {
                        let mut new_params = generics_mut.params.clone();
                        for param in &generics_mut.params {
                            if let GenericParam::Type(type_param) = param {
                                let new_param = GenericParam::Type(TypeParam {
                                    attrs: type_param.attrs.clone(),
                                    default: None,
                                    eq_token: Some(token::Eq::default()),
                                    ident: Ident::new(
                                        &format!("{}_2", type_param.ident),
                                        type_param.ident.span(),
                                    ),
                                    colon_token: type_param.colon_token,
                                    bounds: type_param.bounds.clone(),
                                });
                                new_params.push(new_param);
                            }
                        }
                        generics_mut.params = new_params;
                    }
                }
            } else if let syn::Item::Enum(enum_item) = item {
                if let Some(generics) = &mut enum_item.generics {
                    let generics_mut = generics;
                    if !generics_mut.params.is_empty() {
                        let mut new_params = generics_mut.params.clone();
                        for param in &generics_mut.params {
                            if let GenericParam::Type(type_param) = param {
                                let new_param = GenericParam::Type(TypeParam {
                                    attrs: type_param.attrs.clone(),
                                    default: None,
                                    eq_token: Some(token::Eq::default()),
                                    ident: Ident::new(
                                        &format!("{}_2", type_param.ident),
                                        type_param.ident.span(),
                                    ),
                                    colon_token: type_param.colon_token,
                                    bounds: type_param.bounds.clone(),
                                });
                                new_params.push(new_param);
                            }
                        }
                        generics_mut.params = new_params;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates generic parameters in struct and enum definitions, creating new parameters with the same type and bounds but different names. This transformation tests the handling of repeated generic parameters and may lead to errors in type checking or inference."
    }
}