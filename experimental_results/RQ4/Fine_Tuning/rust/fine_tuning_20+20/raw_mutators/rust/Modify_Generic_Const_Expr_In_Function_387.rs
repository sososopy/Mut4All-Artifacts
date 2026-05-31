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

pub struct Modify_Generic_Const_Expr_In_Function_387;

impl Mutator for Modify_Generic_Const_Expr_In_Function_387 {
    fn name(&self) -> &str {
        "Modify_Generic_Const_Expr_In_Function_387"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_feature = false;
        for attr in &file.attrs {
            if attr.path().is_ident("feature") {
                if let Ok(syn::Meta::List(meta_list)) = attr.parse_meta() {
                    for nested in &meta_list.nested {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested {
                            if path.is_ident("generic_const_exprs") {
                                has_feature = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if !has_feature {
            return;
        }
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if let syn::PathArguments::AngleBracketed(
                                        angle_bracketed_args,
                                    ) = &segment.arguments
                                    {
                                        for arg in &mut angle_bracketed_args.args {
                                            if let syn::GenericArgument::Const(expr) = arg {
                                                if let syn::Expr::Block(expr_block) = expr {
                                                    let mut has_target = false;
                                                    let mut target_ident = None;
                                                    expr_block.block.visit_with(
                                                        &mut |expr: &Expr| {
                                                            if let Expr::Path(expr_path) = expr {
                                                                if let Some(ident) = expr_path
                                                                    .path
                                                                    .get_ident()
                                                                {
                                                                    if ident == "N" {
                                                                        has_target = true;
                                                                        target_ident =
                                                                            Some(ident.clone());
                                                                    }
                                                                }
                                                            }
                                                        },
                                                    );
                                                    if has_target {
                                                        if let Some(ident) = target_ident {
                                                            expr_block.block = parse_quote!({
                                                                #ident * 2
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
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with a `where` clause that includes a generic constant expression, specifically those using the `generic_const_exprs` feature. It modifies the const parameter's expression to introduce a non-linear operation, such as multiplication, involving the same parameter. This transformation aims to create more complex or cyclic dependencies in the const evaluation, potentially exposing compiler bugs related to the stability and correctness of const generic expressions."
    }
}