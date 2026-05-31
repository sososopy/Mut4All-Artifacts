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

pub struct Modify_Associated_Type_Lifetimes_341;

impl Mutator for Modify_Associated_Type_Lifetimes_341 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_Lifetimes_341"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(last_segment) = type_path.path.segments.last() {
                                if let PathArguments::AngleBracketed(angle_bracketed) =
                                    &last_segment.arguments
                                {
                                    let mut new_args = angle_bracketed.clone();
                                    for (i, arg) in angle_bracketed.args.iter().enumerate() {
                                        if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                            if let Some(last_segment) =
                                                type_path.path.segments.last()
                                            {
                                                if let PathArguments::AngleBracketed(
                                                    assoc_args,
                                                ) = &last_segment.arguments
                                                {
                                                    let lifetime_count =
                                                        assoc_args.args.iter().filter(|arg| {
                                                            matches!(
                                                                arg,
                                                                GenericArgument::Lifetime(_)
                                                            )
                                                        }).count();
                                                    let mut new_assoc_args =
                                                        assoc_args.clone();
                                                    if lifetime_count == 0 {
                                                        new_assoc_args.args.push(
                                                            GenericArgument::Lifetime(
                                                                Lifetime::new(
                                                                    "'a",
                                                                    Span::call_site(),
                                                                ),
                                                            ),
                                                        );
                                                    } else if lifetime_count == 1 {
                                                        new_assoc_args.args.clear();
                                                    } else {
                                                        new_assoc_args.args.clear();
                                                        new_assoc_args.args.push(
                                                            GenericArgument::Lifetime(
                                                                Lifetime::new(
                                                                    "'a",
                                                                    Span::call_site(),
                                                                ),
                                                            ),
                                                        );
                                                    }
                                                    new_args.args[i] =
                                                        GenericArgument::Type(Type::Path(
                                                            TypePath {
                                                                qself: None,
                                                                path: SynPath {
                                                                    leading_colon: None,
                                                                    segments: {
                                                                        let mut segments =
                                                                            Punctuated::new();
                                                                        segments.push(
                                                                            PathSegment {
                                                                                ident: last_segment
                                                                                    .ident
                                                                                    .clone(),
                                                                                arguments:
                                                                                    PathArguments::AngleBracketed(
                                                                                        new_assoc_args,
                                                                                    ),
                                                                            },
                                                                        );
                                                                        segments
                                                                    },
                                                                },
                                                            },
                                                        ));
                                                }
                                            }
                                        }
                                    }
                                    let mut new_segments =
                                        type_path.path.segments.clone();
                                    new_segments.pop();
                                    new_segments.push(PathSegment {
                                        ident: last_segment.ident.clone(),
                                        arguments: PathArguments::AngleBracketed(new_args),
                                    });
                                    pat_type.ty = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: type_path.path.leading_colon,
                                            segments: new_segments,
                                        },
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with trait object parameters that specify associated types with lifetime arguments. It modifies the number of lifetime arguments provided to associated types, deliberately creating a mismatch with their expected lifetime parameter count. This transformation aims to exploit the compiler's type checking mechanisms, particularly in scenarios involving trait objects and associated types, to uncover potential bugs related to lifetime resolution and trait system robustness."
    }
}