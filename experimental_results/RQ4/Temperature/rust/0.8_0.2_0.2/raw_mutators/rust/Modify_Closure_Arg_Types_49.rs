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

pub struct Modify_Closure_Arg_Types_49;

impl Mutator for Modify_Closure_Arg_Types_49 {
    fn name(&self) -> &str {
        "Modify_Closure_Arg_Types_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == "FnMut" || segment.ident == "Fn" || segment.ident == "FnOnce" {
                                    if let PathArguments::AngleBracketed(angle_bracketed) = &segment.arguments {
                                        if angle_bracketed.args.len() == 1 {
                                            if let GenericArgument::Type(Type::Tuple(tuple)) = &angle_bracketed.args[0] {
                                                let new_args: Punctuated<GenericArgument, Comma> = tuple.elems.iter().map(|elem| {
                                                    GenericArgument::Type(elem.clone())
                                                }).collect();
                                                let new_segment = PathSegment {
                                                    ident: segment.ident.clone(),
                                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: new_args,
                                                        gt_token: token::Gt::default(),
                                                    }),
                                                };
                                                let new_path = SynPath {
                                                    leading_colon: type_path.path.leading_colon,
                                                    segments: type_path.path.segments.iter().cloned().take(type_path.path.segments.len() - 1).chain(std::iter::once(new_segment)).collect(),
                                                };
                                                pat_type.ty = Box::new(Type::Path(TypePath {
                                                    qself: type_path.qself.clone(),
                                                    path: new_path,
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
    fn chain_of_thought(&self) -> &str {
        ""
    }
}