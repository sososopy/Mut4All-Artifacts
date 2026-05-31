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

pub struct Modify_Poll_Usage_In_Function_225;

impl Mutator for Modify_Poll_Usage_In_Function_225 {
    fn name(&self) -> &str {
        "Modify_Poll_Usage_In_Function_225"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_match = false;
                for stmt in &func.block.stmts {
                    if let syn::Stmt::Expr(expr) = stmt {
                        if let syn::Expr::Match(expr_match) = expr {
                            if let syn::Expr::Path(expr_path) = &*expr_match.expr {
                                if expr_path.path.segments.len() == 1
                                    && expr_path.path.segments[0].ident == "val"
                                {
                                    has_match = true;
                                }
                            }
                        }
                    }
                }
                if !has_match {
                    continue;
                }
                let mut has_custom_type = false;
                for stmt in &func.block.stmts {
                    if let syn::Stmt::Item(item) = stmt {
                        if let syn::Item::Enum(item_enum) = item {
                            if item_enum.ident == "CustomType" {
                                has_custom_type = true;
                            }
                        }
                    }
                }
                if !has_custom_type {
                    let enum_def: syn::Item = parse_quote! {
                        enum CustomType {
                            Value(u32),
                        }
                    };
                    func.block.stmts.insert(0, syn::Stmt::Item(enum_def));
                }
                for input in &mut func.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::Path(type_path) = &*pat_type.ty {
                            if type_path
                                .path
                                .segments
                                .iter()
                                .any(|seg| seg.ident == "Poll")
                            {
                                pat_type.ty = Box::new(parse_quote!(
                                    Poll<Result<Option<CustomType>, u8>>
                                ));
                            }
                        }
                    }
                }
                for stmt in &mut func.block.stmts {
                    if let syn::Stmt::Expr(expr, _) = stmt {
                        if let syn::Expr::Match(expr_match) = expr {
                            if let syn::Expr::Path(expr_path) = &*expr_match.expr {
                                if expr_path.path.segments.len() == 1
                                    && expr_path.path.segments[0].ident == "val"
                                {
                                    for arm in &mut expr_match.arms {
                                        if let syn::Pat::TupleStruct(pat_tuple_struct) = &arm.pat {
                                            if pat_tuple_struct.path.segments.len() == 2
                                                && pat_tuple_struct.path.segments[0].ident == "Poll"
                                                && pat_tuple_struct.path.segments[1].ident == "Ready"
                                            {
                                                if let Some(syn::Pat::TupleStruct(
                                                    inner_pat_tuple_struct,
                                                )) = pat_tuple_struct.pat.elems.first()
                                                {
                                                    if inner_pat_tuple_struct.path.segments.len()
                                                        == 1
                                                        && inner_pat_tuple_struct
                                                            .path
                                                            .segments[0]
                                                            .ident
                                                            == "Ok"
                                                    {
                                                        if let Some(syn::Pat::TupleStruct(
                                                            innermost_pat_tuple_struct,
                                                        )) = inner_pat_tuple_struct
                                                            .pat
                                                            .elems
                                                            .first()
                                                        {
                                                            if innermost_pat_tuple_struct
                                                                .path
                                                                .segments
                                                                .len()
                                                                == 1
                                                                && innermost_pat_tuple_struct
                                                                    .path
                                                                    .segments[0]
                                                                    .ident
                                                                    == "Some"
                                                            {
                                                                arm.pat = parse_quote!(
                                                                    Poll::Ready(Ok(Some(
                                                                        CustomType::Value(_)
                                                                    )))
                                                                );
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
        "The mutator targets functions that match on a `Poll` enum value. It modifies the match arm handling `Poll::Ready(Ok(Some(...)))` to use a `CustomType` enum with a `Value(u32)` variant, ensuring the enum is defined within the function if not already present. This transformation aims to stress-test the compiler's handling of enum pattern matching and type resolution by introducing a new type structure within an existing control flow pattern."
    }
}