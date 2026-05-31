use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Argument_Reorder_Mutator_317;

impl Mutator for Argument_Reorder_Mutator_317 {
    fn name(&self) -> &str {
        "Argument_Reorder_Mutator_317"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Call(call_expr) = &**expr {
                            if let Expr::Path(path_expr) = &*call_expr.func {
                                let args = &mut call_expr.args;
                                if args.len() > 1 {
                                    let mut rng = thread_rng();
                                    let indices: Vec<_> = (0..args.len()).collect();
                                    let mut shuffled_indices: Vec<_> = indices.clone();
                                    shuffled_indices.shuffle(&mut rng);
                                    let mut new_args = Punctuated::new();
                                    for index in shuffled_indices {
                                        new_args.push(args[index].clone());
                                    }
                                    args.clone_from(&new_args);
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr) = stmt {
                                if let Expr::Call(call_expr) = &**expr {
                                    if let Expr::Path(path_expr) = &*call_expr.func {
                                        let args = &mut call_expr.args;
                                        if args.len() > 1 {
                                            let mut rng = thread_rng();
                                            let indices: Vec<_> = (0..args.len()).collect();
                                            let mut shuffled_indices: Vec<_> = indices.clone();
                                            shuffled_indices.shuffle(&mut rng);
                                            let mut new_args = Punctuated::new();
                                            for index in shuffled_indices {
                                                new_args.push(args[index].clone());
                                            }
                                            args.clone_from(&new_args);
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
        "The ArgumentReorder mutator randomly reorders the arguments of function calls to create a mismatch between the caller ABI and caller arguments, similar to the bug reported. It applies to all function calls in the code and aims to trigger errors related to argument ordering and ABI compatibility."
    }
}