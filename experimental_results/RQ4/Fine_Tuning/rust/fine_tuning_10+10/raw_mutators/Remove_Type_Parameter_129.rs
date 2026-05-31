use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Remove_Type_Parameter_129;

impl Mutator for Remove_Type_Parameter_129 {
    fn name(&self) -> &str {
        "Remove_Type_Parameter_129"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(ItemImpl { trait_, items, .. }) = item {
                if let Some((_, path, _)) = trait_ {
                    if let Some(trait_segment) = path.segments.last() {
                        for impl_item in items {
                            if let syn::ImplItem::Method(method) = impl_item {
                                if let Some((_, ref generics, _)) = &method.sig.generics.split_for_impl() {
                                    if !generics.params.is_empty() {
                                        let first_param = generics.params.first().unwrap();
                                        if let syn::GenericParam::Type(type_param) = first_param {
                                            let param_ident = &type_param.ident;
                                            let new_generics: syn::Generics = parse_quote! {};
                                            method.sig.generics = new_generics;
                                            if let Some(trait_segment) = path.segments.last_mut() {
                                                trait_segment.arguments = PathArguments::None;
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
        "This mutation operator targets function implementations for traits, specifically those with type parameters. The mutation involves removing a type parameter from the function signature in the implementation, which is present in the trait signature. This introduces a mismatch between the trait and its implementation, potentially leading to compiler errors due to type parameter discrepancies."
    }
}