use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprMethodCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Mutator_Remove_Type_Params_In_Calls_211;

impl Mutator for Mutator_Remove_Type_Params_In_Calls_211 {
    fn name(&self) -> &str {
        "Mutator_Remove_Type_Params_In_Calls_211"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct RemoveTypeParamsVisitor;
        impl VisitMut for RemoveTypeParamsVisitor {
            fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
                if let Expr::Path(path_expr) = &mut *call.func {
                    if let Some(last_segment) = path_expr.path.segments.last_mut() {
                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                            let mut new_args = Punctuated::new();
                            for arg in args.args.iter_mut() {
                                if let GenericArgument::Type(_) = arg {
                                    // Skip type arguments
                                } else {
                                    new_args.push(std::mem::take(arg));
                                }
                            }
                            args.args = new_args;
                        }
                    }
                }
                visit_mut::VisitMut::visit_expr_call_mut(call);
            }
            fn visit_expr_method_call_mut(&mut self, call: &mut ExprMethodCall) {
                if let Some(last_segment) = call.method.path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        let mut new_args = Punctuated::new();
                        for arg in args.args.iter_mut() {
                            if let GenericArgument::Type(_) = arg {
                                // Skip type arguments
                            } else {
                                new_args.push(std::mem::take(arg));
                            }
                        }
                        args.args = new_args;
                    }
                }
                visit_mut::VisitMut::visit_expr_method_call_mut(call);
            }
        }
        let mut visitor = RemoveTypeParamsVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes explicit type parameters from function and method calls, relying on type inference when the argument's type is unambiguously constrained. This forces the compiler to infer types that were previously explicitly specified, potentially exposing issues in type inference or missing metadata for generic parameters."
    }
}