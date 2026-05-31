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

pub struct Modify_Impl_Trait_Return_Types_In_Functions_164;

impl Mutator for Modify_Impl_Trait_Return_Types_In_Functions_164 {
    fn name(&self) -> &str {
        "Modify_Impl_Trait_Return_Types_In_Functions_164"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut modified = false;
                        let mut new_bounds = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(last_segment) = trait_bound.path.segments.last() {
                                    if last_segment.ident == "Future" {
                                        if let PathArguments::AngleBracketed(
                                            angle_bracketed_args,
                                        ) = &last_segment.arguments
                                        {
                                            let mut new_args = Punctuated::new();
                                            for arg in &angle_bracketed_args.args {
                                                if let GenericArgument::Binding(binding) = arg {
                                                    if binding.ident == "Output" {
                                                        new_args.push(
                                                            GenericArgument::Binding(
                                                                syn::Binding {
                                                                    ident: binding.ident.clone(),
                                                                    eq_token: binding.eq_token,
                                                                    ty: Type::Path(TypePath {
                                                                        qself: None,
                                                                        path: SynPath::from(
                                                                            Ident::new(
                                                                                "T",
                                                                                Span::call_site(),
                                                                            ),
                                                                        ),
                                                                    }),
                                                                },
                                                            ),
                                                        );
                                                        modified = true;
                                                    } else {
                                                        new_args.push(arg.clone());
                                                    }
                                                } else {
                                                    new_args.push(arg.clone());
                                                }
                                            }
                                            let new_future_bound = TraitBound {
                                                paren_token: trait_bound.paren_token,
                                                modifier: trait_bound.modifier.clone(),
                                                lifetimes: trait_bound.lifetimes.clone(),
                                                path: SynPath {
                                                    leading_colon: trait_bound
                                                        .path
                                                        .leading_colon,
                                                    segments: {
                                                        let mut segments = Punctuated::new();
                                                        for segment in
                                                            trait_bound.path.segments.iter()
                                                        {
                                                            if segment.ident == "Future" {
                                                                segments.push(PathSegment {
                                                                    ident: segment.ident.clone(),
                                                                    arguments:
                                                                        PathArguments::AngleBracketed(
                                                                            syn::AngleBracketedGenericArguments {
                                                                                colon2_token:
                                                                                    angle_bracketed_args.colon2_token,
                                                                                lt_token:
                                                                                    angle_bracketed_args.lt_token,
                                                                                args: new_args,
                                                                                gt_token:
                                                                                    angle_bracketed_args.gt_token,
                                                                            },
                                                                        ),
                                                                });
                                                            } else {
                                                                segments.push(segment.clone());
                                                            }
                                                        }
                                                        segments
                                                    },
                                                },
                                            };
                                            new_bounds.push(TypeParamBound::Trait(
                                                new_future_bound,
                                            ));
                                        } else {
                                            new_bounds.push(bound.clone());
                                        }
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        }
                        if modified {
                            item_fn.sig.output = ReturnType::Type(
                                token::RArrow {
                                    spans: [Span::call_site()],
                                },
                                Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: type_impl_trait.impl_token,
                                    bounds: new_bounds,
                                })),
                            );
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with `impl Future<Output = impl Send>` return types, replacing the nested `impl Send` with an undeclared generic `T`. This transformation introduces a type inconsistency by altering the trait bounds within the `impl Trait`, aiming to stress-test the compiler's handling of opaque types and trait resolution in complex return scenarios."
    }
}