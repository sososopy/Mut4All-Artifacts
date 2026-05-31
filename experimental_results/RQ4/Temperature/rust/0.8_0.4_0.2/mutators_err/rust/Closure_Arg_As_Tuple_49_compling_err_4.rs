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

pub struct Closure_Arg_As_Tuple_49;

impl Mutator for Closure_Arg_As_Tuple_49 {
    fn name(&self) -> &str {
        "Closure_Arg_As_Tuple_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_args = Punctuated::new();
                let mut first_arg_type = None;
                if let Some(first_arg) = func.sig.inputs.first_mut() {
                    if let FnArg::Typed(pat_type) = first_arg {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(last_segment) = type_path.path.segments.last() {
                                if last_segment.ident == "FnMut" {
                                    first_arg_type = Some(pat_type);
                                }
                            }
                        }
                    }
                }
                if let Some(pat_type) = first_arg_type {
                    for input in func.sig.inputs.iter().skip(1) {
                        if let FnArg::Typed(arg) = input {
                            if let Type::Path(arg_type_path) = &*arg.ty {
                                new_args.push((*arg.ty).clone());
                            }
                        }
                    }
                    pat_type.ty = Box::new(Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments: {
                                let mut segments = Punctuated::new();
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if let Some(last_segment) = type_path.path.segments.last() {
                                        segments.push(PathSegment {
                                            ident: last_segment.ident.clone(),
                                            arguments: PathArguments::AngleBracketed(
                                                AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: token::Lt::default(),
                                                    args: {
                                                        let mut args = Punctuated::new();
                                                        args.push(GenericArgument::Type(
                                                            Type::Tuple(TypeTuple {
                                                                paren_token: Paren::default(),
                                                                elems: new_args,
                                                            }),
                                                        ));
                                                        args.push(GenericArgument::Type(
                                                            parse_quote!(()),
                                                        ));
                                                        args
                                                    },
                                                    gt_token: token::Gt::default(),
                                                },
                                            ),
                                        });
                                    }
                                }
                                segments
                            },
                        },
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}