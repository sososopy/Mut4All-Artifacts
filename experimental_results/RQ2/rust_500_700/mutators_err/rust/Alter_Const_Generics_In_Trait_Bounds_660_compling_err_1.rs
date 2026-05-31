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

pub struct Alter_Const_Generics_In_Trait_Bounds_660;

impl Mutator for Alter_Const_Generics_In_Trait_Bounds_660 {
    fn name(&self) -> &str {
        "Alter_Const_Generics_In_Trait_Bounds_660"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, generics, _)) = &item_impl.trait_ {
                    for param in &generics.params {
                        if let syn::GenericParam::Const(const_param) = param {
                            if const_param.ty == parse_quote!(usize) {
                                for bound in &mut item_impl.generics.where_clause.iter_mut() {
                                    for predicate in &mut bound.predicates {
                                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                                            if let syn::Type::Array(type_array) = &predicate_type.bounded_ty {
                                                if let syn::Expr::Path(expr_path) = &type_array.len {
                                                    if expr_path.path.segments.len() > 1 {
                                                        type_array.len = syn::Expr::Lit(syn::ExprLit {
                                                            attrs: Vec::new(),
                                                            lit: syn::Lit::Int(syn::LitInt::new("4", Span::call_site())),
                                                        });
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