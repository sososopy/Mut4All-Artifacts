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

pub struct Replace_Pointer_Cast_221;

impl Mutator for Replace_Pointer_Cast_221 {
    fn name(&self) -> &str {
        "Replace_Pointer_Cast_221"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if let PathArguments::AngleBracketed(angle_bracketed) = &type_path.path.segments.last().unwrap().arguments {
                            if angle_bracketed.args.len() > 1 {
                                let mut new_args = Punctuated::new();
                                new_args.push(GenericArgument::Type(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("fn", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                })));
                                angle_bracketed.args = new_args;
                            }
                        }
                    }
                }
            }

            if let syn::Item::Stmt(stmt) = item {
                if let Stmt::Local(local) = stmt {
                    if let Pat::Type(pat_type) = &local.pat {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let PathArguments::AngleBracketed(angle_bracketed) = &type_path.path.segments.last().unwrap().arguments {
                                if angle_bracketed.args.len() > 1 {
                                    let mut new_args = Punctuated::new();
                                    new_args.push(GenericArgument::Type(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("fn", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    })));
                                    angle_bracketed.args = new_args;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets pointer casts in the given code and replaces them with a different type of pointer cast to check if the compiler correctly handles various casting operations."
    }
}