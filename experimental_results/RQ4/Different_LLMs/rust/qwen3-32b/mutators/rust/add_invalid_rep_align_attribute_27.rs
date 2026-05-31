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

pub struct Add_Invalid_Rep_Align_Attribute_27;

impl Mutator for Add_Invalid_Rep_Align_Attribute_27 {
    fn name(&self) -> &str {
        "Add_Invalid_Rep_Align_Attribute_27"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                func.attrs.push(parse_quote!(#[repr(align)]));
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        method.attrs.push(parse_quote!(#[repr(align)]));
                    }
                }
            } else if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        method.attrs.push(parse_quote!(#[repr(align)]));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}