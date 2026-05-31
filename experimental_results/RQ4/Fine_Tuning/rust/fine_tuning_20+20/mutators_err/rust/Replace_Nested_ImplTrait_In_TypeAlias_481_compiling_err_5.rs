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

pub struct Replace_Nested_ImplTrait_In_TypeAlias_481;

impl Mutator for Replace_Nested_ImplTrait_In_TypeAlias_481 {
    fn name(&self) -> &str {
        "Replace_Nested_ImplTrait_In_TypeAlias_481"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(type_alias) = item {
                if let Type::ImplTrait(type_impl_trait) = &*type_alias.ty {
                    for bound in &type_impl_trait.bounds {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            if let Some(segment) = trait_bound.path.segments.last() {
                                if segment.ident == "Iterator" {
                                    if let PathArguments::AngleBracketed(args) =
                                        &segment.arguments
                                    {
                                        let mut new_args = Punctuated::new();
                                        for arg in &args.args {
                                            if let GenericArgument::Constraint(constraint) = arg {
                                                if let Some(segment) =
                                                    constraint.ident.segments.last()
                                                {
                                                    if segment.ident == "Item" {
                                                        if let syn::Expr::Type(expr_type) =
                                                            &constraint.expr
                                                        {
                                                            if let syn::Type::ImplTrait(
                                                                inner_impl_trait,
                                                            ) = &*expr_type.ty
                                                            {
                                                                let replacement_type: syn::Type =
                                                                    parse_quote! {
                                                                        std::vec::IntoIter<usize>
                                                                    };
                                                                let new_expr: syn::Expr =
                                                                    parse_quote! {
                                                                        {
                                                                            let _: #replacement_type;
                                                                            ()
                                                                        }
                                                                    };
                                                                let new_constraint =
                                                                    syn::ExprType {
                                                                        ident: constraint.ident.clone(),
                                                                        expr: Box::new(new_expr),
                                                                        colon_token: constraint
                                                                            .colon_token,
                                                                        bounds: Box::new(
                                                                            syn::Type::Verbatim(
                                                                                quote! {
                                                                                    std::vec::IntoIter<usize>
                                                                                },
                                                                            ),
                                                                        ),
                                                                    };
                                                                new_args.push(
                                                                    GenericArgument::Constraint(
                                                                        new_constraint,
                                                                    ),
                                                                );
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
                                            } else {
                                                new_args.push(arg.clone());
                                            }
                                        }
                                        let new_segment = PathSegment {
                                            ident: segment.ident.clone(),
                                            arguments: PathArguments::AngleBracketed(
                                                AngleBracketedGenericArguments {
                                                    colon2_token: args.colon2_token,
                                                    lt_token: args.lt_token,
                                                    args: new_args,
                                                    gt_token: args.gt_token,
                                                },
                                            ),
                                        };
                                        let mut new_segments = Punctuated::new();
                                        for seg in trait_bound.path.segments.iter().take(
                                            trait_bound.path.segments.len() - 1,
                                        ) {
                                            new_segments.push(seg.clone());
                                        }
                                        new_segments.push(new_segment);
                                        let new_path = syn::Path {
                                            leading_colon: trait_bound.path.leading_colon,
                                            segments: new_segments,
                                        };
                                        let new_trait_bound = TraitBound {
                                            paren_token: trait_bound.paren_token,
                                            modifier: trait_bound.modifier.clone(),
                                            lifetimes: trait_bound.lifetimes.clone(),
                                            path: new_path,
                                        };
                                        let new_bound =
                                            TypeParamBound::Trait(new_trait_bound.clone());
                                        let mut new_bounds = Punctuated::new();
                                        for b in &type_impl_trait.bounds {
                                            if let TypeParamBound::Trait(tb) = b {
                                                if tb.path == trait_bound.path {
                                                    new_bounds.push(new_bound.clone());
                                                } else {
                                                    new_bounds.push(b.clone());
                                                }
                                            } else {
                                                new_bounds.push(b.clone());
                                            }
                                        }
                                        let new_type_impl_trait = TypeImplTrait {
                                            impl_token: type_impl_trait.impl_token,
                                            bounds: new_bounds,
                                        };
                                        let new_type =
                                            Type::ImplTrait(new_type_impl_trait.clone());
                                        type_alias.ty = Box::new(new_type);
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
        "The operator targets type aliases using `impl Trait`, particularly when `impl Trait` is nested. The mutation replaces the inner `impl Trait` with a concrete iterator type. This mutation maintains the trait bound expectations of the outer `impl Trait` while removing the nested `impl Trait`, which is prone to causing bugs. The mutation is designed to provoke changes in how the compiler resolves type information and may expose latent bugs in the handling of type aliasing with `impl Trait`."
    }
}