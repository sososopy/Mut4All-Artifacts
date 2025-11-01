use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, ExprLit, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Modify_Enum_Variant_Pattern_Matching_362;

impl Mutator for Modify_Enum_Variant_Pattern_Matching_362 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_Pattern_Matching_362"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut visitor = EnumPatternVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets enum pattern matching in `if let` and `match` statements, specifically modifying the pattern to introduce a type mismatch by changing the literal used in the pattern to a different type."
    }
}

struct EnumPatternVisitor;

impl VisitMut for EnumPatternVisitor {
    fn visit_expr_match_mut(&mut self, node: &mut ExprMatch) {
        for arm in &mut node.arms {
            if let Pat::TupleStruct(ref mut pat) = arm.pat {
                if let Some(last) = pat.elems.last_mut() {
                    if let Pat::Lit(ref mut lit_pat) = last {
                        if let syn::Expr::Lit(ref mut lit_expr) = lit_pat.expr {
                            if let syn::Lit::Str(ref lit_str) = lit_expr.lit {
                                lit_expr.lit = syn::Lit::Int(syn::LitInt::new("42", lit_str.span()));
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_match_mut(self, node);
    }

    fn visit_expr_let_mut(&mut self, node: &mut ExprLet) {
        if let Pat::TupleStruct(ref mut pat) = *node.pat {
            if let Some(last) = pat.elems.last_mut() {
                if let Pat::Lit(ref mut lit_pat) = last {
                    if let syn::Expr::Lit(ref mut lit_expr) = lit_pat.expr {
                        if let syn::Lit::Str(ref lit_str) = lit_expr.lit {
                            lit_expr.lit = syn::Lit::Int(syn::LitInt::new("42", lit_str.span()));
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_let_mut(self, node);
    }
}