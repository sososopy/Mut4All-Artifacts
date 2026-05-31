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

pub struct Modify_Closure_Arg_Handling_49;

impl Mutator for Modify_Closure_Arg_Handling_49 {
    fn name(&self) -> &str {
        "Modify_Closure_Arg_Handling_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                        if let Expr::Path(ExprPath { ref path, .. }) = *expr_call.func {
                            if path.segments.last().unwrap().ident == "call_mut" {
                                let args = &expr_call.args;
                                if args.len() > 1 {
                                    let tuple_expr: Expr = parse_quote! {
                                        (#args)
                                    };
                                    expr_call.args.clear();
                                    expr_call.args.push(tuple_expr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies closure function calls that pass multiple arguments directly by transforming the call to instead pass a single tuple containing all original arguments. This encourages testing scenarios where the closure's argument handling mechanism is varied, potentially revealing issues when the compiler deals with argument passing in the context of unboxed closures."
    }
}