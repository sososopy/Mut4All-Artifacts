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

pub struct Change_Const_Generic_Type_8;

impl Mutator for Change_Const_Generic_Type_8 {
    fn name(&self) -> &str {
        "Change_Const_Generic_Type_8"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let syn::TraitItem::Const(trait_const) = item {
                        if trait_const.ty == parse_quote!(u64) {
                            trait_const.ty = parse_quote!(usize);
                        } else if trait_const.ty == parse_quote!(usize) {
                            trait_const.ty = parse_quote!(u64);
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, ref mut path, _)) = item_impl.trait_ {
                    for segment in &mut path.segments {
                        if let syn::PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                            for arg in &mut args.args {
                                if let syn::GenericArgument::Const(const_arg) = arg {
                                    if const_arg == &parse_quote!(u64) {
                                        *const_arg = parse_quote!(usize);
                                    } else if const_arg == &parse_quote!(usize) {
                                        *const_arg = parse_quote!(u64);
                                    }
                                }
                            }
                        }
                    }
                }
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Const(impl_const) = impl_item {
                        if impl_const.ty == parse_quote!(u64) {
                            impl_const.ty = parse_quote!(usize);
                        } else if impl_const.ty == parse_quote!(usize) {
                            impl_const.ty = parse_quote!(u64);
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