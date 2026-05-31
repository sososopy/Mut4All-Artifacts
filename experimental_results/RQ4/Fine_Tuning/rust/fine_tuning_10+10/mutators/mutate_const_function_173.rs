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

pub struct Mutate_Const_Function_173;

impl Mutator for Mutate_Const_Function_173 {
    fn name(&self) -> &str {
        "Mutate_Const_Function_173"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    for segment in &mut trait_bound.path.segments {
                                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    if let Expr::Block(expr_block) = expr {
                                                        if let Some(Stmt::Expr(Expr::Call(expr_call), _)) = expr_block.block.stmts.last_mut() {
                                                            if let Expr::Path(expr_path) = &*expr_call.func {
                                                                if let Some(last_segment) = expr_path.path.segments.last() {
                                                                    if last_segment.ident == "inner" {
                                                                        // Modify the const function body
                                                                        *expr_block = parse_quote!({ const fn inner() -> bool { true } inner(); false });
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
        }
    }

    fn chain_of_thought(&self) -> &str {
        "1. Traverse the AST to locate functions with a `where` clause containing a generic constant expression.\n\
         2. Identify the const function invoked within this expression.\n\
         3. Modify the body of the const function to return a different constant value type, such as `bool`.\n\
         4. Adjust the surrounding context to match the new type where necessary."
    }
}