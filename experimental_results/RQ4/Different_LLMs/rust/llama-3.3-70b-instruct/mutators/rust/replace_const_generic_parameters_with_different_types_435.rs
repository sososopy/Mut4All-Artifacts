use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Const_Generic_Parameters_With_Different_Types_435;

impl Mutator for Replace_Const_Generic_Parameters_With_Different_Types_435 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Parameters_With_Different_Types_435"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(param) = param {
                        let new_type = match thread_rng().gen_range(0..3) {
                            0 => parse_quote!(bool),
                            1 => parse_quote!(char),
                            2 => parse_quote!(String),
                            _ => unreachable!(),
                        };
                        param.default = Some(Expr::Path(ExprPath {
                            qself: None,
                            attrs: vec![],
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new("Default", Span::call_site()),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        }));
                        param.ty = new_type;
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(param) = param {
                                let new_type = match thread_rng().gen_range(0..3) {
                                    0 => parse_quote!(bool),
                                    1 => parse_quote!(char),
                                    2 => parse_quote!(String),
                                    _ => unreachable!(),
                                };
                                param.default = Some(Expr::Path(ExprPath {
                                    qself: None,
                                    attrs: vec![],
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("Default", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                }));
                                param.ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with different types (bool, char, String) to test the compiler's handling of various type combinations."
    }
}