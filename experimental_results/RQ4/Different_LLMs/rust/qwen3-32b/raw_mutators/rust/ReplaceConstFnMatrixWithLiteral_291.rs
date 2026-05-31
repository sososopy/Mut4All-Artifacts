use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeArray, TypeImplTrait,
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

pub struct ReplaceConstFnMatrixWithLiteral_291;

impl Mutator for ReplaceConstFnMatrixWithLiteral_291 {
    fn name(&self) -> &str {
        "ReplaceConstFnMatrixWithLiteral_291"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let syn::Type::Path(type_path) = &mut impl_item.self_ty {
                    for segment in &mut type_path.path.segments {
                        if let syn::PathArguments::AngleBracketed(angle_args) = &mut segment.arguments {
                            for arg in &mut angle_args.args {
                                if let syn::GenericArgument::Const(expr) = arg {
                                    if let syn::Expr::Call(call_expr) = &*expr {
                                        // Check if the function call is to a known const function
                                        if let syn::Expr::Path(path_expr) = &*call_expr.func {
                                            if let Some(ident) = path_expr.path.get_ident() {
                                                if ident == "to_matrix" {
                                                    // Extract array type from the const parameter's type
                                                    // Assume it's a 2D array [[T; N]; M]
                                                    // Generate [[0; N]; M]
                                                    let array_expr = {
                                                        let zero_expr = parse_quote!(0);
                                                        let inner_array = ExprArray {
                                                            attrs: vec![],
                                                            bracket_token: token::Bracket::default(),
                                                            elems: Punctuated::from_iter(vec![zero_expr]),
                                                            semi_token: Some(token::Semi::default()),
                                                            len: Some(Box::new(parse_quote!(N))),
                                                        };
                                                        let outer_array = ExprArray {
                                                            attrs: vec![],
                                                            bracket_token: token::Bracket::default(),
                                                            elems: Punctuated::from_iter(vec![Expr::Array(inner_array)]),
                                                            semi_token: Some(token::Semi::default()),
                                                            len: Some(Box::new(parse_quote!(N))),
                                                        };
                                                        outer_array
                                                    };
                                                    *expr = Box::new(array_expr);
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