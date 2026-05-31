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

pub struct Add_Invalid_Trait_Generic_Params_271;

impl Mutator for Add_Invalid_Trait_Generic_Params_271 {
    fn name(&self) -> &str {
        "Add_Invalid_Trait_Generic_Params_271"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, ty) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(impl_trait) = *ty.as_mut() {
                        for bound in &mut impl_trait.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound, modifier) = bound {
                                let mut modified = false;
                                let mut new_segments = syn::punctuated::Punctuated::new();
                                for (i, segment) in trait_bound.path.segments.iter().enumerate() {
                                    new_segments.push(segment.clone());
                                    if i == trait_bound.path.segments.len() - 1 {
                                        if let syn::PathArguments::None = segment.arguments {
                                            modified = true;
                                            let new_args = parse_quote!(<u32>);
                                            let new_segment = syn::PathSegment {
                                                ident: segment.ident.clone(),
                                                arguments: new_args,
                                            };
                                            new_segments.pop();
                                            new_segments.push(new_segment);
                                            break;
                                        }
                                    }
                                }
                                if modified {
                                    trait_bound.path.segments = new_segments;
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        if let syn::ReturnType::Type(_, ty) = &mut method.sig.output {
                            if let syn::Type::ImplTrait(impl_trait) = *ty.as_mut() {
                                for bound in &mut impl_trait.bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound, modifier) = bound {
                                        let mut modified = false;
                                        let mut new_segments = syn::punctuated::Punctuated::new();
                                        for (i, segment) in trait_bound.path.segments.iter().enumerate() {
                                            new_segments.push(segment.clone());
                                            if i == trait_bound.path.segments.len() - 1 {
                                                if let syn::PathArguments::None = segment.arguments {
                                                    modified = true;
                                                    let new_args = parse_quote!(<u32>);
                                                    let new_segment = syn::PathSegment {
                                                        ident: segment.ident.clone(),
                                                        arguments: new_args,
                                                    };
                                                    new_segments.pop();
                                                    new_segments.push(new_segment);
                                                    break;
                                                }
                                            }
                                        }
                                        if modified {
                                            trait_bound.path.segments = new_segments;
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
        "The mutation operator introduces invalid generic parameters to traits in `impl Trait` return types. It identifies traits with no declared generics and appends arbitrary type arguments (e.g., `u32`) to the last path segment, creating a malformed trait bound. This transformation forces the compiler to process a trait with incorrect parameter counts, potentially triggering ICEs during AST traversal or type-checking."
    }
}