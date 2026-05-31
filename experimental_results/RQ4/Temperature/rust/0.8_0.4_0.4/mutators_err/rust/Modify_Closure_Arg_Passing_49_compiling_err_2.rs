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

pub struct Modify_Closure_Arg_Passing_49;

impl Mutator for Modify_Closure_Arg_Passing_49 {
    fn name(&self) -> &str {
        "Modify_Closure_Arg_Passing_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(FnArg::Typed(ref mut pat_type)) = func.sig.inputs.first_mut() {
                    if let syn::Type::Path(type_path) = &mut *pat_type.ty {
                        if let Some(last_segment) = type_path.path.segments.last_mut() {
                            if last_segment.ident == "FnMut" || last_segment.ident == "FnOnce" || last_segment.ident == "Fn" {
                                if let syn::PathArguments::AngleBracketed(ref mut angle_bracketed_args) = last_segment.arguments {
                                    if angle_bracketed_args.args.len() > 1 {
                                        let tuple_args: Type = parse_quote!((#angle_bracketed_args));
                                        angle_bracketed_args.args.clear();
                                        angle_bracketed_args.args.push(GenericArgument::Type(tuple_args));
                                    }
                                }
                            }
                        }
                    }
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Call(ref mut call_expr), _) = stmt {
                        if let Expr::Path(ref path_expr) = *call_expr.func {
                            if let Some(last_segment) = path_expr.path.segments.last() {
                                if last_segment.ident == "closure" && call_expr.args.len() > 1 {
                                    let tuple_expr: Expr = parse_quote!((#call_expr.args));
                                    call_expr.args.clear();
                                    call_expr.args.push(tuple_expr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies function definitions that utilize closures, ensuring closure arguments are passed as a tuple. It targets functions where closures are called with multiple arguments, transforming them into a single tuple argument. This tests the handling of tuple-based argument passing in closures, potentially exposing bugs related to argument unpacking and tuple handling in unboxed closures."
    }
}