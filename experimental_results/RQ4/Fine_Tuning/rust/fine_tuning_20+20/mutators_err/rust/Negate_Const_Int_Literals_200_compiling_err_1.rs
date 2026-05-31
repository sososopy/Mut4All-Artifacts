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

pub struct Negate_Const_Int_Literals_200;

fn negate_integer_literal(lit: &str) -> Option<String> {
    let int_re = Regex::new(r"^(\d+)(_.*)?$").unwrap();
    let hex_re = Regex::new(r"^0[xX]([0-9a-fA-F]+)(_.*)?$").unwrap();
    let bin_re = Regex::new(r"^0[bB]([01]+)(_.*)?$").unwrap();
    let oct_re = Regex::new(r"^0[oO]?([0-7]+)(_.*)?$").unwrap();
    if let Some(caps) = int_re.captures(lit) {
        let value = caps.get(1)?.as_str().parse::<i128>().ok()?;
        let suffix = caps.get(2).map_or("", |m| m.as_str());
        Some(format!("-{}{}", value, suffix))
    } else if let Some(caps) = hex_re.captures(lit) {
        let value = i128::from_str_radix(caps.get(1)?.as_str(), 16).ok()?;
        let suffix = caps.get(2).map_or("", |m| m.as_str());
        Some(format!("-0x{:x}{}", value, suffix))
    } else if let Some(caps) = bin_re.captures(lit) {
        let value = i128::from_str_radix(caps.get(1)?.as_str(), 2).ok()?;
        let suffix = caps.get(2).map_or("", |m| m.as_str());
        Some(format!("-0b{:b}{}", value, suffix))
    } else if let Some(caps) = oct_re.captures(lit) {
        let value = i128::from_str_radix(caps.get(1)?.as_str(), 8).ok()?;
        let suffix = caps.get(2).map_or("", |m| m.as_str());
        Some(format!("-0o{:o}{}", value, suffix))
    } else {
        None
    }
}

impl Mutator for Negate_Const_Int_Literals_200 {
    fn name(&self) -> &str {
        "Negate_Const_Int_Literals_200"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut negatable_literals = Vec::new();
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &local.init {
                            if let Expr::Lit(expr_lit) = expr.as_ref() {
                                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                    if let Some(negated) =
                                        negate_integer_literal(&lit_int.to_string())
                                    {
                                        negatable_literals.push((lit_int.clone(), negated));
                                    }
                                }
                            }
                        }
                    }
                }
                let mut rng = thread_rng();
                if let Some((target_lit, negated)) = negatable_literals.choose(&mut rng) {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some((_, expr)) = &mut local.init {
                                if let Expr::Lit(expr_lit) = expr.as_mut() {
                                    if let syn::Lit::Int(lit_int) = &mut expr_lit.lit {
                                        if lit_int == target_lit {
                                            *lit_int = syn::LitInt::new(
                                                &negated,
                                                target_lit.span(),
                                            );
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
        "The mutator targets integer literals in function bodies and attempts to negate them. It identifies negatable integer literals, including decimal, hexadecimal, binary, and octal forms, and replaces one of them with its negative equivalent. This transformation aims to explore edge cases related to integer negation, particularly focusing on potential overflow scenarios and the compiler's handling of negative literals."
    }
}