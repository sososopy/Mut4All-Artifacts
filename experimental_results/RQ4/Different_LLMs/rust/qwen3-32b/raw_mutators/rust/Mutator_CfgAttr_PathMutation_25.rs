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

pub struct Mutator_CfgAttr_PathMutation_25;

impl Mutator for Mutator_CfgAttr_PathMutation_25 {
    fn name(&self) -> &str {
        "Mutator_CfgAttr_PathMutation_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            for attr in &mut item.attrs {
                if let Ok(meta) = attr.parse_meta() {
                    if let syn::Meta::List(list) = meta {
                        if list.path.is_ident("cfg_attr") {
                            let nested = &list.nested;
                            if nested.len() >= 2 {
                                let second = &nested[1];
                                if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = second {
                                    let mut new_segments = Punctuated::new();
                                    new_segments.push(syn::PathSegment {
                                        ident: Ident::new("cfg_attr", Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    for seg in &path.segments {
                                        new_segments.push(seg.clone());
                                    }
                                    let new_path = SynPath {
                                        leading_colon: None,
                                        segments: new_segments,
                                    };
                                    let new_tokens = quote!(#new_path);
                                    let new_attr = syn::Attribute::parse_outer(&new_tokens.into())
                                        .unwrap()
                                        .remove(0);
                                    *attr = new_attr;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms valid `cfg_attr` macro invocations into incorrect path-based attributes by replacing `cfg_attr!(...)` with `cfg_attr::...`. This forces the compiler to resolve `cfg_attr` as a module or crate path rather than a macro, exposing gaps in attribute resolution logic. The transformation is applied to all existing `cfg_attr` usages in attributes, ensuring broad coverage of attribute processing code."
    }
}