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

pub struct Replace_Type_Parameter_With_Trait_Object_436;

impl Mutator for Replace_Type_Parameter_With_Trait_Object_436 {
    fn name(&self) -> &str {
        "Replace_Type_Parameter_With_Trait_Object_436"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            let trait_bound = TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("Trait", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            };
                            let type_param_bound = TypeParamBound::Trait(trait_bound);
                            type_param.bounds.push(type_param_bound);
                        }
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            let trait_bound = TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("Trait", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            };
                            let type_param_bound = TypeParamBound::Trait(trait_bound);
                            type_param.bounds.push(type_param_bound);
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            let trait_bound = TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("Trait", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            };
                            let type_param_bound = TypeParamBound::Trait(trait_bound);
                            type_param.bounds.push(type_param_bound);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type parameters with trait objects. This transformation can help expose bugs related to non-lifetime binders by changing the way the type system handles type parameters."
    }
}