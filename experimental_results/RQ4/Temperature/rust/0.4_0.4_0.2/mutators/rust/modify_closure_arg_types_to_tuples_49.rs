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

pub struct Modify_Closure_Arg_Types_To_Tuples_49;

impl Mutator for Modify_Closure_Arg_Types_To_Tuples_49 {
    fn name(&self) -> &str {
        "Modify_Closure_Arg_Types_To_Tuples_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Path(type_path) = &mut predicate_type.bounded_ty {
                                if let Some(segment) = type_path.path.segments.last_mut() {
                                    if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                                        if let PathArguments::AngleBracketed(angle_bracketed) = &mut segment.arguments {
                                            let mut new_args = Punctuated::new();
                                            let mut tuple_elems = Punctuated::new();
                                            for arg in &angle_bracketed.args {
                                                if let GenericArgument::Type(ty) = arg {
                                                    tuple_elems.push(ty.clone());
                                                }
                                            }
                                            new_args.push(GenericArgument::Type(Type::Tuple(syn::TypeTuple {
                                                paren_token: token::Paren::default(),
                                                elems: tuple_elems,
                                            })));
                                            segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: token::Lt::default(),
                                                args: new_args,
                                                gt_token: token::Gt::default(),
                                            });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                        if let Expr::Path(expr_path) = &*expr_call.func {
                            if let Some(segment) = expr_path.path.segments.last() {
                                if segment.ident == "closure" {
                                    let mut new_args = Punctuated::new();
                                    let mut tuple_args = Punctuated::new();
                                    for arg in &expr_call.args {
                                        tuple_args.push(arg.clone());
                                    }
                                    new_args.push(Expr::Tuple(syn::ExprTuple {
                                        attrs: Vec::new(),
                                        paren_token: token::Paren::default(),
                                        elems: tuple_args,
                                    }));
                                    expr_call.args = new_args;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function definitions with closure arguments using Fn, FnMut, or FnOnce traits. It modifies the argument types to use tuples, which requires adjusting the function calls to pass tuples instead of individual arguments. This transformation tests the Rust compiler's handling of closures with tuple arguments, potentially uncovering issues with closure unboxing or tuple trait management."
    }
}