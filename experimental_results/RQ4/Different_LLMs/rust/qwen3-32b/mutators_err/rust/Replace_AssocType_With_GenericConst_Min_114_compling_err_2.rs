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

pub struct Replace_AssocType_With_GenericConst_Min_114;

impl Mutator for Replace_AssocType_With_GenericConst_Min_114 {
    fn name(&self) -> &str {
        "Replace_AssocType_With_GenericConst_Min_114"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                let impl_generics = &impl_item.generics;
                let const_params: Vec<_> = impl_generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let syn::GenericParam::Const(const_param) = param {
                            Some(const_param.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if const_params.len() < 2 {
                    continue;
                }
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(assoc_type) = impl_item {
                        if let syn::Type::Path(_) = assoc_type.ty.as_ref() {
                            let c1 = &const_params[0].ident;
                            let c2 = &const_params[1].ident;
                            let min_call = syn::ExprCall {
                                attrs: Vec::new(),
                                func: Box::new(syn::Expr::Path(syn::ExprPath {
                                    attrs: Vec::new(),
                                    qself: None,
                                    path: syn::Path::from(Ident::new("min", Span::call_site())),
                                })),
                                paren_token: Default::default(),
                                args: {
                                    let mut args = syn::punctuated::Punctuated::new();
                                    args.push(syn::Expr::Path(syn::ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: syn::Path::from(c1.clone()),
                                    }));
                                    args.push(syn::Expr::Path(syn::ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: syn::Path::from(c2.clone()),
                                    }));
                                    args
                                },
                            };
                            let t_type = syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new("T", Span::call_site())),
                            });
                            let array_type = syn::Type::Array(Box::new(syn::TypeArray {
                                bracket_token: Default::default(),
                                elem: Box::new(t_type),
                                len: Box::new(min_call),
                                semi_token: Default::default(),
                            }));
                            *assoc_type.ty = array_type;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces an associated type in a trait implementation with a generic const expression using an existing const function on the struct's const parameters. It leverages an existing min-like function to compute the const expression, transforming the associated type into an array with a length determined by the minimum of two const parameters. This tests the compiler's handling of const expressions in trait associated types and dynamic const evaluation in generic contexts."
    }
}