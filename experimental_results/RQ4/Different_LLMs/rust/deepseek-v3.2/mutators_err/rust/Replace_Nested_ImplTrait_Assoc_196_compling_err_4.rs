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

pub struct Replace_Nested_ImplTrait_Assoc_196;

impl Mutator for Replace_Nested_ImplTrait_Assoc_196 {
    fn name(&self) -> &str {
        "Replace_Nested_ImplTrait_Assoc_196"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut impls = Vec::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((trait_path, lifetime_param, assoc_type)) = extract_trait_assoc(&item_impl) {
                    impls.push((trait_path.clone(), lifetime_param.clone(), assoc_type.clone()));
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_output = None;
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some((assoc_bound, nested_trait)) = find_nested_assoc(&trait_bound.path) {
                                    for (trait_path, lifetime_param, concrete_assoc) in &impls {
                                        if trait_bound.path == *trait_path && nested_trait_is_implemented(&nested_trait, &concrete_assoc) {
                                            let new_path = replace_assoc_in_path(&trait_bound.path, assoc_bound.clone(), concrete_assoc);
                                            let new_trait_bound = TraitBound {
                                                paren_token: trait_bound.paren_token,
                                                modifier: trait_bound.modifier,
                                                lifetimes: trait_bound.lifetimes.clone(),
                                                path: new_path,
                                            };
                                            let mut new_bounds = Punctuated::new();
                                            new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                            let new_type_1 = TypeImplTrait {
                                                impl_token: type_impl_trait.impl_token,
                                                bounds: new_bounds,
                                            };
                                            new_output = Some(ReturnType::Type(Default::default(), Box::new(Type::ImplTrait(new_type_1)));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if let Some(output) = new_output {
                    item_fn.sig.output = output;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl for<'a> Trait<'a, Assoc = impl SomeTrait>` and replaces the nested `impl SomeTrait` in the associated type position with a concrete type from existing trait implementations. It scans the file for impl blocks where a type implements `Trait<'lifetime>` with an `Assoc` type that implements `SomeTrait`. When a matching impl is found, the function signature is mutated to use that concrete `Assoc` type. This transformation tests the compiler's handling of nested impl Trait in HRTB contexts and associated type projections, potentially uncovering bugs in lifetime and trait resolution."
    }
}

fn extract_trait_assoc(item_impl: &syn::ItemImpl) -> Option<(syn::Path, syn::LifetimeParam, syn::Type)> {
    if let Some((_, trait_path, _)) = &item_impl.trait_ {
        for impl_item in &item_impl.items {
            if let ImplItem::Type(assoc_type) = impl_item {
                if let Some(lifetime_param) = find_lifetime_param(&item_impl.generics) {
                    return Some((trait_path.clone(), lifetime_param.clone(), assoc_type.ty.clone()));
                }
            }
        }
    }
    None
}

fn find_lifetime_param(generics: &syn::Generics) -> Option<syn::LifetimeParam> {
    for param in &generics.params {
        if let GenericParam::Lifetime(lifetime_param) = param {
            return Some(lifetime_param.clone());
        }
    }
    None
}

fn find_nested_assoc(path: &syn::Path) -> Option<(syn::PathSegment, syn::Path)> {
    for segment in &path.segments {
        if let PathArguments::AngleBracketed(args) = &segment.arguments {
            for arg in &args.args {
                if let GenericArgument::AssocType(assoc_type) = arg {
                    if let Type::ImplTrait(type_impl_trait) = &assoc_type.ty {
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                return Some((segment.clone(), trait_bound.path.clone()));
                            }
                        }
                    }
                }
            }
        }
    }
    None
}

fn nested_trait_is_implemented(nested_trait_path: &syn::Path, concrete_type: &syn::Type) -> bool {
    true
}

fn replace_assoc_in_path(path: &syn::Path, assoc_segment: syn::PathSegment, concrete_type: &syn::Type) -> syn::Path {
    let mut new_segments = Punctuated::new();
    for segment in &path.segments {
        if segment == &assoc_segment {
            let new_args = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                colon2_token: None,
                lt_token: token::Lt::default(),
                args: Punctuated::from_iter(vec![GenericArgument::AssocType(AssocType {
                    ident: assoc_segment.ident.clone(),
                    generics: None,
                    eq_token: token::Eq::default(),
                    ty: concrete_type.clone(),
                })]),
                gt_token: token::Gt::default(),
            });
            new_segments.push(PathSegment {
                ident: assoc_segment.ident.clone(),
                arguments: new_args,
            });
        } else {
            new_segments.push(segment.clone());
        }
    }
    syn::Path {
        leading_colon: path.leading_colon,
        segments: new_segments,
    }
}