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

pub struct Remove_Const_Assoc_Bound_327;

impl Mutator for Remove_Const_Assoc_Bound_327 {
    fn name(&self) -> &str {
        "Remove_Const_Assoc_Bound_327"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if trait_item.attrs.iter().any(|attr| attr.path.is_ident("const_trait")) {
                    for trait_item in &mut trait_item.items {
                        if let syn::TraitItem::Type(ty) = trait_item {
                            for bound in &mut ty.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if let syn::TraitBoundModifier::Const = trait_bound.modifier {
                                        trait_bound.modifier = syn::TraitBoundModifier::None;
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