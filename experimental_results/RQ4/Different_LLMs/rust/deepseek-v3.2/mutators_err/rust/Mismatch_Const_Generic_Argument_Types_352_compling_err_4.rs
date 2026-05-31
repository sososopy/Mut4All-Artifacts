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

pub struct Mismatch_Const_Generic_Argument_Types_352;

impl Mutator for Mismatch_Const_Generic_Argument_Types_352 {
    fn name(&self) -> &str {
        "Mismatch_Const_Generic_Argument_Types_352"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MismatchConstGenericVisitor {
            rng: thread_rng(),
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic arguments in function calls, replacing them with const expressions of different integer types while preserving semantic value. It identifies numeric literals or simple constant expressions in const generic positions and transforms them by adding type suffixes or inserting casts to mismatched types (e.g., usize to i32). This stresses the compiler's const generic type checking and evaluation during monomorphization, potentially triggering internal compiler errors at the boundary of polymorphic constant visitation."
    }
}

struct MismatchConstGenericVisitor {
    rng: rand::rngs::ThreadRng,
    mutated: bool,
}

impl VisitMut for MismatchConstGenericVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &mut *node.func {
            if let Some(segment) = path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(const_expr) = arg {
                            self.mutate_const_generic_argument(const_expr);
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }

    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Path(ExprPath { path, .. }) = node {
            if let Some(segment) = path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(const_expr) = arg {
                            self.mutate_const_generic_argument(const_expr);
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}

impl MismatchConstGenericVisitor {
    fn mutate_const_generic_argument(&mut self, expr: &mut Expr) {
        if self.mutated {
            return;
        }
        let candidate_types = vec!["usize", "isize", "u32", "i32", "u64", "i64", "u8", "i8", "u16", "i16"];
        let target_type = candidate_types.choose(&mut self.rng).unwrap();
        let alt_type = candidate_types.choose(&mut self.rng).unwrap();
        if target_type == alt_type {
            return;
        }

        match expr {
            Expr::Lit(expr_lit) => {
                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                    let value = lit_int.base10_digits();
                    let new_expr: Expr = parse_quote! { { #value as #alt_type as #target_type } };
                    *expr = new_expr;
                    self.mutated = true;
                }
            }
            Expr::Path(ExprPath { path, .. }) => {
                let new_expr: Expr = parse_quote! { { #path as #alt_type as #target_type } };
                *expr = new_expr;
                self.mutated = true;
            }
            Expr::Cast(expr_cast) => {
                let new_expr: Expr = parse_quote! { { #expr_cast as #alt_type as #target_type } };
                *expr = new_expr;
                self.mutated = true;
            }
            _ => {}
        }
    }
}