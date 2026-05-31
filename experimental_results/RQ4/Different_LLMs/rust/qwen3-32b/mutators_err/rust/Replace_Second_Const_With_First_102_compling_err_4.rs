use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, boxed::Box};
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

pub struct Replace_Second_Const_With_First_102;

impl Mutator for Replace_Second_Const_With_First_102 {
    fn name(&self) -> &str {
        "Replace_Second_Const_With_First_102"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                        if let Some(last_segment) = type_path.path.segments.last() {
                            if let syn::PathArguments::AngleBracketed(angle_args) = &last_segment.arguments {
                                if angle_args.args.len() == 2 {
                                    let first_arg = angle_args.args[0].clone();
                                    let second_arg = angle_args.args[1].clone();
                                    if let syn::GenericArgument::Const(first_expr) = first_arg {
                                        if let syn::GenericArgument::Const(second_expr) = second_arg {
                                            if let syn::Expr::Path(expr_path) = &*second_expr {
                                                if expr_path.path.segments.len() == 1 {
                                                    let ident = expr_path.path.segments[0].ident.clone();
                                                    for impl_item in &mut impl_item.items {
                                                        if let syn::ImplItem::Const(const_item) = impl_item {
                                                            if let Some(expr) = &*const_item.expr {
                                                                if let syn::Expr::Path(expr_path2) = expr {
                                                                    if expr_path2.path.segments.len() == 1 {
                                                                        let expr_ident = expr_path2.path.segments[0].ident.clone();
                                                                        if expr_ident == ident {
                                                                            const_item.expr = Some(*first_expr);
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}