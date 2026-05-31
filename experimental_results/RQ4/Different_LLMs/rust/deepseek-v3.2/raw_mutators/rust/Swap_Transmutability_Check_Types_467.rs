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

pub struct Swap_Transmutability_Check_Types_467;

impl Mutator for Swap_Transmutability_Check_Types_467 {
    fn name(&self) -> &str {
        "Swap_Transmutability_Check_Types_467"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = TransmutabilityVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets transmutability check function calls by swapping the order of their type arguments. It identifies function calls with two type arguments in contexts related to transmutability (e.g., functions named 'is_maybe_transmutable' or using the BikeshedIntrinsicFrom trait) and reverses their positions. This transformation tests the compiler's handling of transmutability checks with swapped source and destination types, potentially revealing bugs in type compatibility analysis, trait resolution, or transmutability validation logic."
    }
}

struct TransmutabilityVisitor;

impl VisitMut for TransmutabilityVisitor {
    fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
            let last_segment = path.segments.last();
            if let Some(last_segment) = last_segment {
                let func_name = last_segment.ident.to_string();
                if func_name.contains("transmutable") || func_name.contains("transmut") || func_name.contains("is_maybe") {
                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        let type_args: Vec<_> = args.args.iter().filter(|arg| matches!(arg, GenericArgument::Type(_))).collect();
                        if type_args.len() == 2 {
                            let mut new_args = Punctuated::new();
                            let mut type_arg_iter = args.args.iter();
                            for arg in &args.args {
                                match arg {
                                    GenericArgument::Type(_) => {
                                        if new_args.is_empty() {
                                            if let Some(second_type) = type_args.get(1) {
                                                new_args.push(second_type.clone());
                                            }
                                        } else {
                                            if let Some(first_type) = type_args.get(0) {
                                                new_args.push(first_type.clone());
                                            }
                                        }
                                    }
                                    _ => new_args.push(arg.clone()),
                                }
                            }
                            args.args = new_args;
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, expr_call);
    }
}