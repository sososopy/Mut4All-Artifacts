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

pub struct Replace_Generic_Constraint_With_Supertrait_294;

impl Mutator for Replace_Generic_Constraint_With_Supertrait_294 {
    fn name(&self) -> &str {
        "Replace_Generic_Constraint_With_Supertrait_294"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let supertrait_path = SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new(
                                            &format!("Super{}", trait_bound.path.segments.last().unwrap().ident),
                                            Span::call_site(),
                                        ),
                                        arguments: PathArguments::None,
                                    }]),
                                };
                                trait_bound.path = supertrait_path;
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                for bound in &mut type_param.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        let supertrait_path = SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new(
                                                    &format!("Super{}", trait_bound.path.segments.last().unwrap().ident),
                                                    Span::call_site(),
                                                ),
                                                arguments: PathArguments::None,
                                            }]),
                                        };
                                        trait_bound.path = supertrait_path;
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
        "The mutation operator replaces generic constraints with their supertraits. This transformation aims to test the compiler's handling of trait bounds and associated types by introducing relaxed constraints, potentially leading to bugs related to type inference and trait resolution."
    }
}