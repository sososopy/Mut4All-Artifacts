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

pub struct Substitute_Default_Generic_Parameter_365;

impl Mutator for Substitute_Default_Generic_Parameter_365 {
    fn name(&self) -> &str {
        "Substitute_Default_Generic_Parameter_365"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for param in &mut trait_item.generics.params {
                    if let GenericParam::Const(param) = param {
                        if let Some((_, expr)) = &param.default {
                            let new_expr = match expr {
                                Expr::Binary(expr) => {
                                    if let (Expr::Path(path1), Expr::Path(path2)) = (&*expr.left, &*expr.right) {
                                        if let (Some(ident1), Some(ident2)) = (path1.path.segments.last().map(|s| &s.ident), path2.path.segments.last().map(|s| &s.ident)) {
                                            if ident1 == "X" && ident2 == "Y" {
                                                parse_quote!({ X * Y })
                                            } else {
                                                expr.clone()
                                            }
                                        } else {
                                            expr.clone()
                                        }
                                    } else {
                                        expr.clone()
                                    }
                                }
                                _ => expr.clone(),
                            };
                            param.default = Some((token::Eq::default(), new_expr));
                        }
                    }
                }
            }

            if let Item::Struct(struct_item) = item {
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Const(param) = param {
                        if let Some((_, expr)) = &param.default {
                            let new_expr = match expr {
                                Expr::Binary(expr) => {
                                    if let (Expr::Path(path1), Expr::Path(path2)) = (&*expr.left, &*expr.right) {
                                        if let (Some(ident1), Some(ident2)) = (path1.path.segments.last().map(|s| &s.ident), path2.path.segments.last().map(|s| &s.ident)) {
                                            if ident1 == "X" && ident2 == "Y" {
                                                parse_quote!({ X * Y })
                                            } else {
                                                expr.clone()
                                            }
                                        } else {
                                            expr.clone()
                                        }
                                    } else {
                                        expr.clone()
                                    }
                                }
                                _ => expr.clone(),
                            };
                            param.default = Some((token::Eq::default(), new_expr));
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes default generic parameters in trait and struct definitions with new expressions, aiming to test the compiler's handling of different default values and trigger potential bugs."
    }
}