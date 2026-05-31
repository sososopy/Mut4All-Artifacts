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

pub struct Introduce_Trait_Bounds_On_Nested_Impl_Trait_15;

impl Mutator for Introduce_Trait_Bounds_On_Nested_Impl_Trait_15 {
    fn name(&self) -> &str {
        "Introduce_Trait_Bounds_On_Nested_Impl_Trait_15"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut has_nested_impl_trait = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.segments.iter().any(|seg| {
                                    if let PathArguments::AngleBracketed(args) = &seg.arguments {
                                        args.args.iter().any(|arg| {
                                            if let GenericArgument::Type(Type::ImplTrait(_)) = arg {
                                                has_nested_impl_trait = true;
                                                true
                                            } else {
                                                false
                                            }
                                        })
                                    } else {
                                        false
                                    }
                                }) {
                                    break;
                                }
                            }
                        }
                        if has_nested_impl_trait {
                            type_impl_trait.bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segments = Punctuated::new();
                                        segments.push(PathSegment {
                                            ident: Ident::new("Clone", Span::call_site()),
                                            arguments: PathArguments::None,
                                        });
                                        segments
                                    },
                                },
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with nested `impl Trait` return types and adds a `Clone` trait bound to the outer `impl Trait`. This transformation tests the compiler's ability to enforce additional trait constraints on complex return types, potentially revealing issues in trait resolution and type inference."
    }
}