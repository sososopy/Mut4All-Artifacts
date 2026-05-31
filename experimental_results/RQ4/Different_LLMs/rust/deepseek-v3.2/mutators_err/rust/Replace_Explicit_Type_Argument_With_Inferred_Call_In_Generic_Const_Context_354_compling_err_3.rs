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

pub struct Replace_Explicit_Type_Argument_With_Inferred_Call_In_Generic_Const_Context_354;

impl Mutator for Replace_Explicit_Type_Argument_With_Inferred_Call_In_Generic_Const_Context_354 {
    fn name(&self) -> &str {
        "Replace_Explicit_Type_Argument_With_Inferred_Call_In_Generic_Const_Context_354"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceExplicitTypeVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function calls with explicit generic type arguments (turbofish syntax) within generic functions that have where clauses involving const expressions dependent on associated constants. It removes the explicit type arguments, forcing the compiler to infer the types. This transformation aims to expose bugs in const evaluation during type inference, particularly with the generic_const_exprs feature, where the compiler may fail to handle associated constants in const generic contexts when types are inferred."
    }
}

struct ReplaceExplicitTypeVisitor;

impl VisitMut for ReplaceExplicitTypeVisitor {
    fn visit_expr_call_mut(&mut self, expr: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*expr.func {
            if let PathArguments::AngleBracketed(args) = &path.segments.last().unwrap().arguments {
                if !args.args.is_empty() {
                    let mut new_path = path.clone();
                    if let Some(last_seg) = new_path.segments.last_mut() {
                        last_seg.arguments = PathArguments::None;
                    }
                    expr.func = Box::new(Expr::Path(ExprPath { qself: None, path: new_path, attrs: vec![] }));
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, expr);
    }
}