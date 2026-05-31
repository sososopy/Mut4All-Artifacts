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

pub struct Replace_Trait_Bounds_With_Supertraits_276;

impl Mutator for Replace_Trait_Bounds_With_Supertraits_276 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_With_Supertraits_276"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for bound in &mut trait_item.supertraits {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(path) = trait_bound.path.get() {
                            let supertraits = get_supertraits(path);
                            if let Some(supertrait) = supertraits.choose(&mut thread_rng()) {
                                *bound = TypeParamBound::Trait(TraitBound {
                                    paren_token: trait_bound.paren_token,
                                    lifetimes: trait_bound.lifetimes.clone(),
                                    path: supertrait.clone(),
                                });
                            }
                        }
                    }
                }
            } else if let Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = &mut impl_item.trait_ {
                    for segment in &mut path.segments {
                        match &mut segment.arguments {
                            PathArguments::AngleBracketed(args) => {
                                for arg in &mut args.args {
                                    if let GenericArgument::Type(Type::Path(path)) = arg {
                                        let supertraits = get_supertraits(path);
                                        if let Some(supertrait) = supertraits.choose(&mut thread_rng()) {
                                            *arg = GenericArgument::Type(Type::Path(supertrait.clone()));
                                        }
                                    }
                                }
                            }
                            _ => {}
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds with their supertraits. This transformation tests the robustness of the trait system and can expose bugs related to trait bounds and supertraits."
    }
}

fn get_supertraits(path: &SynPath) -> Vec<SynPath> {
    let mut supertraits = Vec::new();
    match path.segments.last().unwrap().ident.to_string().as_str() {
        "Iterator" => {
            supertraits.push(parse_quote!(std::iter::ExactSizeIterator));
            supertraits.push(parse_quote!(std::iter::DoubleEndedIterator));
        }
        "ExactSizeIterator" => {
            supertraits.push(parse_quote!(std::iter::Iterator));
        }
        "DoubleEndedIterator" => {
            supertraits.push(parse_quote!(std::iter::Iterator));
        }
        _ => {}
    }
    supertraits
}