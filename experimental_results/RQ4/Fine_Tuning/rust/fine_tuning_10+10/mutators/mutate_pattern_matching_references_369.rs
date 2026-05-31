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
};

use crate::mutator::Mutator;

pub struct Mutate_Pattern_Matching_References_369;

impl Mutator for Mutate_Pattern_Matching_References_369 {
    fn name(&self) -> &str {
        "Mutate_Pattern_Matching_References_369"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct PatternMutator;

        impl VisitMut for PatternMutator {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                match expr {
                    Expr::If(expr_if) => {
                        if let Expr::Let(expr_let) = &mut *expr_if.cond {
                            if let Pat::Reference(pat_ref) = &mut *expr_let.pat {
                                if pat_ref.mutability.is_none() {
                                    pat_ref.mutability = Some(token::Mut(Span::call_site()));
                                } else {
                                    pat_ref.mutability = None;
                                }
                            }
                        }
                    }
                    Expr::Match(expr_match) => {
                        for arm in &mut expr_match.arms {
                            if let Pat::Reference(pat_ref) = &mut arm.pat {
                                if pat_ref.mutability.is_none() {
                                    pat_ref.mutability = Some(token::Mut(Span::call_site()));
                                } else {
                                    pat_ref.mutability = None;
                                }
                            }
                        }
                    }
                    _ => {}
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        PatternMutator.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets pattern matching constructs in Rust, specifically within `if let` and `match` expressions. It identifies patterns involving references and mutable references, and swaps one instance of a reference pattern (`&`) with a mutable reference pattern (`&mut`), or vice versa. This alteration tests the compiler's handling of pattern mutability, especially in nested structures, ensuring that the syntax and logic remain consistent and valid."
    }
}