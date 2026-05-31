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

pub struct Replace_Trait_Bound_With_Different_Trait_Bound_346;

impl Mutator for Replace_Trait_Bound_With_Different_Trait_Bound_346 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_With_Different_Trait_Bound_346"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = param {
                        if let Type::Path(type_path) = &mut *pat_type.ty {
                            for segment in &mut type_path.path.segments {
                                if let PathArguments::AngleBracketed(angle_bracketed) = &mut segment.arguments {
                                    for arg in &mut angle_bracketed.args {
                                        if let GenericArgument::Type(Type::Path(type_path_inner)) = arg {
                                            for segment_inner in &mut type_path_inner.path.segments {
                                                if let PathArguments::AngleBracketed(angle_bracketed_inner) = &mut segment_inner.arguments {
                                                    for arg_inner in &mut angle_bracketed_inner.args {
                                                        if let GenericArgument::Type(Type::TraitObject(trait_object)) = arg_inner {
                                                            for bound in &mut trait_object.bounds {
                                                                if let TypeParamBound::Trait(trait_bound) = bound {
                                                                    let new_trait_bound = TraitBound {
                                                                        paren_token: None,
                                                                        modifier: TraitBoundModifier::None,
                                                                        lifetimes: None,
                                                                        path: syn::Path::from(Ident::new("Debug", Span::call_site())),
                                                                    };
                                                                    *trait_bound = new_trait_bound;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = param {
                                if let Type::Path(type_path) = &mut *pat_type.ty {
                                    for segment in &mut type_path.path.segments {
                                        if let PathArguments::AngleBracketed(angle_bracketed) = &mut segment.arguments {
                                            for arg in &mut angle_bracketed.args {
                                                if let GenericArgument::Type(Type::Path(type_path_inner)) = arg {
                                                    for segment_inner in &mut type_path_inner.path.segments {
                                                        if let PathArguments::AngleBracketed(angle_bracketed_inner) = &mut segment_inner.arguments {
                                                            for arg_inner in &mut angle_bracketed_inner.args {
                                                                if let GenericArgument::Type(Type::TraitObject(trait_object)) = arg_inner {
                                                                    for bound in &mut trait_object.bounds {
                                                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                                                            let new_trait_bound = TraitBound {
                                                                                paren_token: None,
                                                                                modifier: TraitBoundModifier::None,
                                                                                lifetimes: None,
                                                                                path: syn::Path::from(Ident::new("Debug", Span::call_site())),
                                                                            };
                                                                            *trait_bound = new_trait_bound;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
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
        "The mutation operator replaces trait bounds in generic type or function definitions with different trait bounds, potentially triggering bugs related to generic const expressions, such as out-of-range errors or incorrect type substitutions."
    }
}