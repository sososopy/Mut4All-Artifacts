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

pub struct Negate_Integer_Literals_469;

impl Mutator for Negate_Integer_Literals_469 {
    fn name(&self) -> &str {
        "Negate_Integer_Literals_469"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        struct LiteralNegator;
        
        impl VisitMut for LiteralNegator {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Lit(ExprLit { lit: Lit::Int(lit_int), .. }) = expr {
                    if let Some(ty) = lit_int.suffix() {
                        let value = lit_int.base10_parse::<i128>().unwrap_or_default();
                        if value > 0 {
                            let negated_value = -value;
                            let negated_lit = LitInt::new(&format!("{}", negated_value), lit_int.span());
                            *expr = Expr::Lit(ExprLit {
                                attrs: vec![],
                                lit: Lit::Int(negated_lit),
                            });
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut negator = LiteralNegator;
                negator.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets integer literals within function bodies. It identifies integer literals and negates their values, specifically focusing on compile-time constants. This transformation aims to test the compiler's handling of integer overflow and negative literals, potentially leading to ICEs or overflow errors when the mutated literals exceed the type's limits."
    }
}