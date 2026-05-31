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

pub struct Mutate_Deref_DispatchFromDyn_Trait_185;

impl Mutator for Mutate_Deref_DispatchFromDyn_Trait_185 {
    fn name(&self) -> &str {
        "Mutate_Deref_DispatchFromDyn_Trait_185"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                let mut has_deref_self = false;
                for bound in &item_trait.supertraits {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        let path = &trait_bound.path;
                        if path.is_ident("Deref") {
                            if let Some(last_segment) = path.segments.last() {
                                if let PathArguments::AngleBracketed(args) =
                                    &last_segment.arguments
                                {
                                    for arg in &args.args {
                                        if let GenericArgument::Constraint(constraint) = arg {
                                            if constraint.ident == "Target" {
                                                if constraint
                                                    .bounds
                                                    .iter()
                                                    .any(|bound| matches!(bound, TypeParamBound::Trait(trait_bound) if trait_bound.path.is_ident("Self")))
                                                {
                                                    has_deref_self = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if has_deref_self {
                    let mut new_supertraits = Punctuated::new();
                    for bound in &item_trait.supertraits {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            let path = &trait_bound.path;
                            if path.is_ident("Deref") {
                                if let Some(last_segment) = path.segments.last() {
                                    if let PathArguments::AngleBracketed(args) =
                                        &last_segment.arguments
                                    {
                                        let mut new_args = Punctuated::new();
                                        for arg in &args.args {
                                            if let GenericArgument::Constraint(constraint) = arg {
                                                if constraint.ident == "Target" {
                                                    let new_constraint: syn::Constraint =
                                                        syn::parse_quote!(Target = u32);
                                                    new_args.push(syn::GenericArgument::Constraint(
                                                        new_constraint,
                                                    ));
                                                } else {
                                                    new_args.push(arg.clone());
                                                }
                                            } else {
                                                new_args.push(arg.clone());
                                            }
                                        }
                                        let new_segment = PathSegment {
                                            ident: last_segment.ident.clone(),
                                            arguments:
                                                PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: args.colon2_token,
                                                        lt_token: args.lt_token,
                                                        args: new_args,
                                                        gt_token: args.gt_token,
                                                    },
                                                ),
                                        };
                                        let mut new_segments = path.segments.clone();
                                        *new_segments.last_mut().unwrap() = new_segment;
                                        let new_path = syn::Path {
                                            leading_colon: path.leading_colon,
                                            segments: new_segments,
                                        };
                                        new_supertraits.push(TypeParamBound::Trait(
                                            TraitBound {
                                                paren_token: trait_bound.paren_token,
                                                modifier: trait_bound.modifier.clone(),
                                                lifetimes: trait_bound.lifetimes.clone(),
                                                path: new_path,
                                            },
                                        ));
                                    } else {
                                        new_supertraits.push(bound.clone());
                                    }
                                } else {
                                    new_supertraits.push(bound.clone());
                                }
                            } else {
                                new_supertraits.push(bound.clone());
                            }
                        } else {
                            new_supertraits.push(bound.clone());
                        }
                    }
                    item_trait.supertraits = new_supertraits;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait definitions that use `Deref` and `DispatchFromDyn` with `Self` as the target type. By replacing `Self` with a concrete type like `u32`, it alters the trait's type constraints, potentially exposing compiler issues related to trait resolution and type inference in complex trait hierarchies."
    }
}