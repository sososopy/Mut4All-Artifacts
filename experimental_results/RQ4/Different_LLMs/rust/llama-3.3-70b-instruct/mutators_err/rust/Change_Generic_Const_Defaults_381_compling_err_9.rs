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

pub struct Change_Generic_Const_Defaults_381;

impl Mutator for Change_Generic_Const_Defaults_381 {
    fn name(&self) -> &str {
        "Change_Generic_Const_Defaults_381"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                match &mut struct_item.generics {
                    syn::Generics { where_clause, .. } => {
                        if let Some(where_clause) = where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate) = predicate {
                                    for bound in &mut predicate.bounds {
                                        match bound {
                                            TypeParamBound::Trait(TraitBound { path, .. }) => {
                                                if let Some(segment) = path.segments.last() {
                                                    if segment.ident.to_string() == "Const" {
                                                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                                            if let Some(arg) = args.args.first() {
                                                                if let GenericArgument::Const(Expr::Path(path)) = arg {
                                                                    let new_default_value = match path {
                                                                        ExprPath { path, .. } => {
                                                                            Expr::Path(ExprPath {
                                                                                attrs: vec![],
                                                                                qself: None,
                                                                                path: SynPath {
                                                                                    leading_colon: None,
                                                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                                                        ident: Ident::new("u8", Span::call_site()),
                                                                                        arguments: PathArguments::None,
                                                                                    }]),
                                                                                },
                                                                            })
                                                                        }
                                                                        _ => unreachable!(),
                                                                    };
                                                                    segment.arguments = Some(PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                                        colon2_token: token::Colon2::default(),
                                                                        lt_token: token::Lt::default(),
                                                                        args: Punctuated::from_iter(vec![GenericArgument::Const(new_default_value)]),
                                                                        gt_token: token::Gt::default(),
                                                                    }));
                                                                } else {
                                                                    let new_default_value = match arg {
                                                                        GenericArgument::Const(Expr::Lit(lit)) => {
                                                                            Expr::Lit(ExprLit {
                                                                                attrs: vec![],
                                                                                lit: Lit::Bool(LitBool { span: Span::call_site(), value: true }),
                                                                            })
                                                                        }
                                                                        _ => unreachable!(),
                                                                    };
                                                                    segment.arguments = Some(PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                                        colon2_token: token::Colon2::default(),
                                                                        lt_token: token::Lt::default(),
                                                                        args: Punctuated::from_iter(vec![GenericArgument::Const(new_default_value)]),
                                                                        gt_token: token::Gt::default(),
                                                                    }));
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            _ => {}
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
        "The mutation operator targets the default values of generic constants in structs. It changes the default value of a generic constant to a different type or an invalid value, potentially exposing bugs in the compiler's handling of generic constants and their default values."
    }
}