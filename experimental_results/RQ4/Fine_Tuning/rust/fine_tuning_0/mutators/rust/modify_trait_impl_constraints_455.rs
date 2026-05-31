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

pub struct Modify_Trait_Impl_Constraints_455;

impl Mutator for Modify_Trait_Impl_Constraints_455 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_Constraints_455"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.last().unwrap().ident == "Assoc" {
                        let mut generics = item_impl.generics.clone();
                        if let Some(ref mut type_param) = generics.params.first_mut() {
                            if let syn::GenericParam::Type(type_param) = type_param {
                                type_param.bounds.clear();
                            }
                        }
                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Type(type_item) = impl_item {
                                if type_item.ident == "Output" {
                                    type_item.ty = parse_quote!(u32);
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