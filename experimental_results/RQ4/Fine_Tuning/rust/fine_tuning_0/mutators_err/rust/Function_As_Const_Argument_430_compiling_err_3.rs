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
    Attribute, Meta, MetaList, WhereClause, PredicateType, TypeArray, TypeInfer,
    parse::Parse,
};

use crate::mutator::Mutator;

pub struct Function_As_Const_Argument_430;

impl Mutator for Function_As_Const_Argument_430 {
    fn name(&self) -> &str {
        "Function_As_Const_Argument_430"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_feature = false;
        for attr in &file.attrs {
            if attr.path().is_ident("feature") {
                if let Ok(meta) = attr.parse_args_with(Meta::parse) {
                    if let Meta::List(meta_list) = meta {
                        for token in meta_list.tokens {
                            if let Some(ident) = token.ident() {
                                if ident == "min_generic_const_args" {
                                    has_feature = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        if !has_feature {
            file.attrs.push(parse_quote!(#![feature(min_generic_const_args)]));
        }

        let mut function_name = None;

        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident != "main" {
                    function_name = Some(item_fn.sig.ident.clone());
                    break;
                }
            }
        }

        if let Some(func_name) = function_name {
            for item in &mut file.items {
                if let Item::Struct(item_struct) = item {
                    if item_struct.generics.params.is_empty() {
                        item_struct.generics.params.push(parse_quote!(T));
                    }

                    item_struct.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(syn::WherePredicate::Type(PredicateType {
                                lifetimes: None,
                                bounded_ty: syn::Type::Array(syn::TypeArray {
                                    bracket_token: Default::default(),
                                    elem: Box::new(syn::Type::Infer(syn::TypeInfer {
                                        underscore_token: Default::default(),
                                    })),
                                    len: syn::Expr::Call(syn::ExprCall {
                                        attrs: Vec::new(),
                                        func: Box::new(syn::Expr::Path(syn::ExprPath {
                                            attrs: Vec::new(),
                                            qself: None,
                                            path: syn::Path::from(func_name.clone()),
                                        })),
                                        paren_token: Default::default(),
                                        args: Punctuated::new(),
                                    }),
                                    semi_token: Default::default(),
                                }),
                                colon_token: Default::default(),
                                bounds: Punctuated::new(),
                            }));
                            predicates
                        },
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}