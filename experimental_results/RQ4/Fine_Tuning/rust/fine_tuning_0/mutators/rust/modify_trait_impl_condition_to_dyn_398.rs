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

pub struct Modify_Trait_Impl_Condition_To_Dyn_398;

impl Mutator for Modify_Trait_Impl_Condition_To_Dyn_398 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_Condition_To_Dyn_398"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(impl_type) = impl_item {
                        if let Some(where_clause) = &mut item_impl.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    for bound in &mut predicate_type.bounds {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            if trait_bound.path.is_ident("IsTrue") {
                                                if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                                                    if let syn::PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                                        for arg in &mut args.args {
                                                            if let syn::GenericArgument::Const(expr) = arg {
                                                                if let syn::Expr::Block(expr_block) = expr {
                                                                    if let Some(first_stmt) = expr_block.block.stmts.first_mut() {
                                                                        if let syn::Stmt::Expr(expr, _) = first_stmt {
                                                                            if let syn::Expr::Binary(expr_binary) = expr {
                                                                                if let syn::Expr::Call(expr_call) = &*expr_binary.left {
                                                                                    if let syn::Expr::Path(expr_path) = &*expr_call.func {
                                                                                        if expr_path.path.is_ident("std::mem::size_of") {
                                                                                            expr_binary.left = Box::new(parse_quote!(std::mem::size_of::<dyn Trait>()));
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}