use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Modify_Const_Generics_Default_365;

impl Mutator for Modify_Const_Generics_Default_365 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_Default_365"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                if let Some(generics) = item_trait.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            const_param.default = None;
                            Some(&item_trait.generics)
                        } else {
                            None
                        }
                    } else {
                        None
                    }
                }) {
                    for impl_item in &mut file.items {
                        if let Item::Impl(item_impl) = impl_item {
                            if let Some((_, trait_ref, _)) = &item_impl.trait_ {
                                if trait_ref.segments.last().unwrap().ident == item_trait.ident {
                                    let const_params: Vec<_> = generics.params.iter().filter_map(|param| {
                                        if let GenericParam::Const(const_param) = param {
                                            Some(const_param.ident.clone())
                                        } else {
                                            None
                                        }
                                    }).collect();

                                    let new_args: Punctuated<GenericArgument, Comma> = const_params.iter().map(|ident| {
                                        parse_quote!(#ident)
                                    }).collect::<Punctuated<GenericArgument, Comma>>();

                                    if let PathArguments::AngleBracketed(ref mut args) = trait_ref.segments.last_mut().unwrap().arguments {
                                        args.args = new_args;
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