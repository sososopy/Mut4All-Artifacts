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

pub struct Introduce_Unconstrained_Type_Parameters_36;

impl Mutator for Introduce_Unconstrained_Type_Parameters_36 {
    fn name(&self) -> &str {
        "Introduce_Unconstrained_Type_Parameters_36"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut has_unconstrained = false;
                for param in &item_impl.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        if !item_impl.trait_.is_some() || !item_impl.self_ty.to_token_stream().to_string().contains(&type_param.ident.to_string()) {
                            has_unconstrained = true;
                            break;
                        }
                    }
                }
                if !has_unconstrained {
                    item_impl.generics.params.push(parse_quote!(K));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}