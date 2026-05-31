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

pub struct Replace_Generic_Const_Args_With_Negation_100;

impl Mutator for Replace_Generic_Const_Args_With_Negation_100 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_Args_With_Negation_100"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Default = func.sig.output {
                    for arg in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = arg {
                            if let Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            }) = &*pat_type.ty
                            {
                                if let Some(segment) = segments.first() {
                                    if segment.ident == "from_options" {
                                        for arg in &mut func.sig.inputs {
                                            if let FnArg::Typed(pat_type) = arg {
                                                if let Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath {
                                                        leading_colon: None,
                                                        segments: inner_segments,
                                                    },
                                                }) = &*pat_type.ty
                                                {
                                                    if let Some(inner_segment) = inner_segments.first() {
                                                        if inner_segment.ident == "bool" {
                                                            pat_type.ty = Box::new(Type::Path(TypePath {
                                                                qself: None,
                                                                path: SynPath {
                                                                    leading_colon: None,
                                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                                        ident: Ident::new(
                                                                            "not",
                                                                            Span::call_site(),
                                                                        ),
                                                                        arguments: PathArguments::None,
                                                                    }]),
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
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic constant arguments with their negation in const generic functions, specifically targeting const generic functions that use the generic const exprs feature. It replaces bool const arguments with their opposite, using variables and types that already exist in the seed program, and avoids introducing new elements unless necessary for the mutation."
    }
}