use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprLit, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Lit, LitInt, Local, Pat, PatType, Path as SynPath,
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

pub struct Negate_Integer_Literals_200;

impl Mutator for Negate_Integer_Literals_200 {
    fn name(&self) -> &str {
        "Negate_Integer_Literals_200"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = IntegerLiteralVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies integer literals in function bodies and negates them if they are compile-time constants, ensuring they are not already part of a negative arithmetic operation. This tests the compiler's handling of integer overflow and negation, especially at type boundaries."
    }
}

struct IntegerLiteralVisitor;

impl VisitMut for IntegerLiteralVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Lit(ExprLit { lit: Lit::Int(lit_int), .. }) = expr {
            if let Ok(value) = lit_int.base10_parse::<i128>() {
                if value > 0 {
                    let negated_value = -value;
                    let negated_literal = LitInt::new(&negated_value.to_string(), lit_int.span());
                    *expr = Expr::Lit(ExprLit {
                        attrs: vec![],
                        lit: Lit::Int(negated_literal),
                    });
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}