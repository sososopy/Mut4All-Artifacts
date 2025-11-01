use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Function_Call_161;

impl Mutator for Replace_Const_Function_Call_161 {
    fn name(&self) -> &str {
        "Replace_Const_Function_Call_161"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(first_segment) = path.segments.first() {
                        if let PathArguments::AngleBracketed(args) = &first_segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    if let Expr::Call(ExprCall { func, .. }) = expr {
                                        if let Expr::Path(ExprPath { path: func_path, .. }) = &**func {
                                            if func_path.segments.len() == 1 {
                                                let new_expr: Expr = parse_quote! { 100 };
                                                *expr = new_expr;
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
        "The mutation operator targets function definitions and calls, specifically those involving const generics and const functions. It identifies const function calls used within trait bounds or function bodies and replaces them with a direct constant value that is unexpected or out of range. This mutation is designed to introduce logical inconsistencies or type mismatches, potentially triggering compiler errors or misinterpretations."
    }
}