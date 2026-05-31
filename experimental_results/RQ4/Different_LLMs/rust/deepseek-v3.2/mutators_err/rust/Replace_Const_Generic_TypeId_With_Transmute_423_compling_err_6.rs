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

pub struct Replace_Const_Generic_TypeId_With_Transmute_423;

impl Mutator for Replace_Const_Generic_TypeId_With_Transmute_423 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_TypeId_With_Transmute_423"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct Visitor;
        impl VisitMut for Visitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Call(call) = expr {
                    if let Expr::Path(path) = &*call.func {
                        if path.path.segments.len() == 1 {
                            if let Some(last_seg) = path.path.segments.last() {
                                if last_seg.ident == "of" {
                                    if let PathArguments::AngleBracketed(args) = &last_seg.arguments {
                                        if args.args.len() == 1 {
                                            if let Some(GenericArgument::Type(_)) = args.args.first() {
                                                let new_expr: Expr = parse_quote! {
                                                    unsafe { std::mem::transmute::<*const _, TypeId>(std::ptr::null()) }
                                                };
                                                *expr = new_expr;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        let mut visitor = Visitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const blocks containing expressions like `TypeId::of::<T>()`. It replaces such calls with `unsafe { std::mem::transmute::<*const _, TypeId>(std::ptr::null()) }`. This transformation uses existing functions but creates a non-constant-evaluable expression, potentially triggering compiler bugs related to const evaluation, generic type handling, and inline const contexts."
    }
}