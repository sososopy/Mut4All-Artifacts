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

pub struct Replace_Bounded_Iterator_With_Infinite_384;

impl Mutator for Replace_Bounded_Iterator_With_Infinite_384 {
    fn name(&self) -> &str {
        "Replace_Bounded_Iterator_With_Infinite_384"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &**return_type {
                        for bound in &type_impl_trait.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Iterator") {
                                    let mut visitor = RangeReplacer;
                                    visitor.visit_block_mut(&mut func.block);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning nested `impl Iterator` types where the innermost iterator's item contains a const parameter. It replaces bounded iterator ranges (e.g., `0..1`) with infinite loops (`loop {}`), preserving const parameter usage in array sizes. This transformation disrupts iteration bounds, potentially exposing bugs in iterator fusion, opaque type inference, and const parameter handling in nested generic contexts."
    }
}

struct RangeReplacer;

impl VisitMut for RangeReplacer {
    fn visit_expr(&mut self, expr: &mut Expr) {
        if let Expr::Range(range_expr) = expr {
            if let Some(start) = &range_expr.from {
                if let Expr::Lit(start_lit) = &**start {
                    if let syn::Lit::Int(start_int) = &start_lit.lit {
                        if let Ok(0) = start_int.base10_parse::<u64>() {
                            if let Some(end) = &range_expr.to {
                                if let Expr::Lit(end_lit) = &**end {
                                    if let syn::Lit::Int(end_int) = &end_lit.lit {
                                        if let Ok(1) = end_int.base10_parse::<u64>() {
                                            *expr = parse_quote! { loop {} };
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            syn::visit_mut::visit_expr_range(self, range_expr);
        } else {
            syn::visit_mut::visit_expr(self, expr);
        }
    }
}