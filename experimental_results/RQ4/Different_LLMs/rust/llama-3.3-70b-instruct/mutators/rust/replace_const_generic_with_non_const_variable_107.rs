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

pub struct Replace_Const_Generic_With_Non_Const_Variable_107;

impl Mutator for Replace_Const_Generic_With_Non_Const_Variable_107 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Non_Const_Variable_107"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                for param in generics.params.iter_mut() {
                    if let GenericParam::Const(param) = param {
                        if let Some(ty) = &param.default {
                            if let Expr::Path(path) = ty {
                                if let Some(segment) = path.path.segments.iter().next() {
                                    if segment.ident == "usize" {
                                        param.default = None;
                                        let new_param = FnArg::Typed(PatType {
                                            attrs: Default::default(),
                                            pat: Box::new(Pat::Ident(PatIdent {
                                                attrs: Default::default(),
                                                by_ref: None,
                                                mutability: Default::default(),
                                                ident: Ident::new(&param.ident.to_string(), param.ident.span()),
                                                subpat: None,
                                            })),
                                            colon_token: Default::default(),
                                            ty: Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new("usize", param.ident.span()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            })),
                                        });
                                        func.sig.inputs.push(new_param);
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
        "The mutation operator replaces const generics with non-const variables of the same type. This transformation aims to test the compiler's handling of const generics and their interaction with non-const variables, potentially leading to type mismatches or inference failures."
    }
}