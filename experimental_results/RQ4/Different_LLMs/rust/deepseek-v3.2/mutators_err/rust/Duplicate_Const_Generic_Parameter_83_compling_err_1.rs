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

pub struct Duplicate_Const_Generic_Parameter_83;

impl Mutator for Duplicate_Const_Generic_Parameter_83 {
    fn name(&self) -> &str {
        "Duplicate_Const_Generic_Parameter_83"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                let const_params = trait_def.generics.params.iter()
                    .filter_map(|p| match p {
                        GenericParam::Const(c) => Some(c),
                        _ => None,
                    })
                    .collect::<Vec<_>>();
                if const_params.is_empty() {
                    continue;
                }
                let chosen = const_params.choose(&mut thread_rng());
                if let Some(chosen_const) = chosen {
                    let ident = &chosen_const.ident;
                    let ty = &chosen_const.ty;
                    let new_param = parse_quote!(const #ident: #ty = #ident);
                    trait_def.generics.params.push(new_param);
                }
            }
            if let syn::Item::Impl(impl_def) = item {
                let const_params = impl_def.generics.params.iter()
                    .filter_map(|p| match p {
                        GenericParam::Const(c) => Some(c),
                        _ => None,
                    })
                    .collect::<Vec<_>>();
                if const_params.is_empty() {
                    continue;
                }
                let chosen = const_params.choose(&mut thread_rng());
                if let Some(chosen_const) = chosen {
                    let ident = &chosen_const.ident;
                    let ty = &chosen_const.ty;
                    let new_param = parse_quote!(const #ident: #ty = #ident);
                    impl_def.generics.params.push(new_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates a const generic parameter in trait or inherent impl definitions, adding a new const parameter with the same name and type as an existing one, and provides a default value referencing the original parameter. This creates duplicate identifiers in the generic parameter list, which can cause ambiguity, shadowing, or conflicts in const generic resolution. It aims to stress the compiler's handling of duplicate const generic names and default value references, potentially triggering ICEs or semantic analysis errors."
    }
}