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

pub struct Modify_Closure_Argument_Passing_49;

impl Mutator for Modify_Closure_Argument_Passing_49 {
    fn name(&self) -> &str {
        "Modify_Closure_Argument_Passing_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &mut *pat_type.ty {
                            if let Some(last_segment) = type_path.path.segments.last_mut() {
                                if last_segment.ident == "FnMut" || last_segment.ident == "Fn" || last_segment.ident == "FnOnce" {
                                    if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                                        let mut tuple_args = Punctuated::new();
                                        for arg in &args.args {
                                            if let GenericArgument::Type(ty) = arg {
                                                tuple_args.push(ty.clone());
                                            }
                                        }
                                        args.args.clear();
                                        args.args.push(GenericArgument::Type(Type::Tuple(TypeTuple {
                                            paren_token: token::Paren::default(),
                                            elems: tuple_args,
                                        })));
                                    }
                                }
                            }
                        }
                    }
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                        if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                            if let Some(last_segment) = path.segments.last() {
                                if last_segment.ident == "closure" {
                                    let mut tuple_exprs = Punctuated::new();
                                    for arg in &expr_call.args {
                                        tuple_exprs.push(arg.clone());
                                    }
                                    expr_call.args.clear();
                                    expr_call.args.push(Expr::Tuple(ExprTuple {
                                        attrs: Vec::new(),
                                        paren_token: token::Paren::default(),
                                        elems: tuple_exprs,
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies how arguments are passed to closures using the unboxed_closures feature. It changes the argument passing from individual arguments to a single tuple, or unpacks if already a tuple. This transformation tests the compiler's handling of closure arguments and tuple unpacking, potentially revealing issues in argument handling and type inference."
    }
}