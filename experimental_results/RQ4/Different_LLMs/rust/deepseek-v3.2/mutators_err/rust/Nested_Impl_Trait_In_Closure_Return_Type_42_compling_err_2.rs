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

pub struct Nested_Impl_Trait_In_Closure_Return_Type_42;

impl Mutator for Nested_Impl_Trait_In_Closure_Return_Type_42 {
    fn name(&self) -> &str {
        "Nested_Impl_Trait_In_Closure_Return_Type_42"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let return_type_clone = item_fn.sig.output.clone();
                if let ReturnType::Type(_, return_type) = &return_type_clone {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let bounds = &type_impl_trait.bounds;
                        for bound in bounds.iter() {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let path = &trait_bound.path;
                                if path.segments.len() == 1 {
                                    let segment = &path.segments[0];
                                    if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                                        if let PathArguments::AngleBracketed(angle_args) = &segment.arguments {
                                            let args = &angle_args.args;
                                            if args.len() > 0 {
                                                let mut new_args = Punctuated::new();
                                                let mut mutated = false;
                                                for arg in args.iter() {
                                                    if let GenericArgument::Type(arg_type) = arg {
                                                        if !mutated {
                                                            if let Type::Path(type_path) = arg_type {
                                                                let path = &type_path.path;
                                                                if path.segments.len() == 1 {
                                                                    let ident = &path.segments[0].ident;
                                                                    let trait_name = Ident::new("Debug", Span::call_site());
                                                                    let new_bound = TypeParamBound::Trait(TraitBound {
                                                                        paren_token: None,
                                                                        modifier: TraitBoundModifier::None,
                                                                        lifetimes: None,
                                                                        path: SynPath::from(trait_name),
                                                                    });
                                                                    let new_type = Type::ImplTrait(TypeImplTrait {
                                                                        impl_token: token::Impl { span: Span::call_site() },
                                                                        bounds: Punctuated::from_iter(vec![new_bound]),
                                                                    });
                                                                    new_args.push(GenericArgument::Type(new_type));
                                                                    mutated = true;
                                                                } else {
                                                                    new_args.push(arg.clone());
                                                                }
                                                            } else {
                                                                new_args.push(arg.clone());
                                                            }
                                                        } else {
                                                            new_args.push(arg.clone());
                                                        }
                                                    } else {
                                                        new_args.push(arg.clone());
                                                    }
                                                }
                                                if mutated {
                                                    let new_segment = PathSegment {
                                                        ident: segment.ident.clone(),
                                                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: angle_args.lt_token.clone(),
                                                            args: new_args,
                                                            gt_token: angle_args.gt_token.clone(),
                                                        }),
                                                    };
                                                    let new_path = SynPath {
                                                        leading_colon: None,
                                                        segments: Punctuated::from_iter(vec![new_segment]),
                                                    };
                                                    let new_trait_bound = TraitBound {
                                                        paren_token: trait_bound.paren_token.clone(),
                                                        modifier: trait_bound.modifier.clone(),
                                                        lifetimes: trait_bound.lifetimes.clone(),
                                                        path: new_path,
                                                    };
                                                    let new_bounds = Punctuated::from_iter(vec![TypeParamBound::Trait(new_trait_bound)]);
                                                    let new_return_type = Type::ImplTrait(TypeImplTrait {
                                                        impl_token: type_impl_trait.impl_token.clone(),
                                                        bounds: new_bounds,
                                                    });
                                                    item_fn.sig.output = ReturnType::Type(token::RArrow::default(), Box::new(new_return_type));
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
        "The mutation operator targets functions returning an impl Trait that is a function trait (Fn, FnMut, FnOnce). It modifies the closure's parameter list by replacing a concrete parameter type with an impl Debug trait, creating nested impl Trait within the closure return type. This transformation aims to replicate conditions that trigger ICEs related to nested impl Trait in closure signatures, testing compiler handling of complex trait bounds and closure type inference."
    }
}