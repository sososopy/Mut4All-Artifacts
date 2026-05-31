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

pub struct Modify_Trait_Fn_Repr_Align_27;

impl Mutator for Modify_Trait_Fn_Repr_Align_27 {
    fn name(&self) -> &str {
        "Modify_Trait_Fn_Repr_Align_27"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(trait_fn) = trait_item {
                        let mut has_repr_align = false;
                        for attr in &trait_fn.attrs {
                            if attr.path().is_ident("repr") {
                                has_repr_align = true;
                                break;
                            }
                        }
                        if !has_repr_align {
                            trait_fn.attrs.push(parse_quote!(#[repr(align)]));
                        } else {
                            for attr in &mut trait_fn.attrs {
                                if attr.path().is_ident("repr") {
                                    attr.meta = syn::Meta::List(syn::MetaList {
                                        path: parse_quote!(repr),
                                        paren_token: syn::token::Paren(Span::call_site()),
                                        nested: parse_quote!(align),
                                    });
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