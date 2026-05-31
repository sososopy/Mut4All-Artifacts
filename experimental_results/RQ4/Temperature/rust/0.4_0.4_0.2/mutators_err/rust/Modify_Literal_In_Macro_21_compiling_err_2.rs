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
    Macro,
    ExprLit,
    Lit,
};

use crate::mutator::Mutator;

pub struct Modify_Literal_In_Macro_21;

impl Mutator for Modify_Literal_In_Macro_21 {
    fn name(&self) -> &str {
        "Modify_Literal_In_Macro_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Ok(mac_body) = mac.mac.parse_body::<Macro>() {
                    for stmt in &mut mac_body.stmts {
                        if let Stmt::Expr(Expr::Lit(expr_lit), _) = stmt {
                            let new_literal = match &expr_lit.lit {
                                Lit::Int(lit_int) => {
                                    let value: i64 = lit_int.base10_parse().unwrap_or(0);
                                    if value < 1_000_000 {
                                        Lit::Int(syn::LitInt::new(&format!("{}", value * 10), lit_int.span()))
                                    } else {
                                        Lit::Float(syn::LitFloat::new(&format!("{}.0", value), lit_int.span()))
                                    }
                                }
                                Lit::Float(lit_float) => {
                                    let value: f64 = lit_float.base10_parse().unwrap_or(0.0);
                                    Lit::Int(syn::LitInt::new(&format!("{}", value as i64), lit_float.span()))
                                }
                                _ => continue,
                            };
                            *stmt = Stmt::Expr(Expr::Lit(ExprLit {
                                attrs: vec![],
                                lit: new_literal,
                            }), None);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macros defined with `macro_rules!` that return literal values. It modifies integer literals to either larger integers or converts them to floating-point numbers, and vice versa for floating-point literals. This tests the compiler's handling of different literal types and ranges, potentially exposing issues with linting, diagnostics, or out-of-range literals."
    }
}