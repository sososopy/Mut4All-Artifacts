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

pub struct Insert_Never_Pattern_In_Guarded_Match_Arm_245;

impl Mutator for Insert_Never_Pattern_In_Guarded_Match_Arm_245 {
    fn name(&self) -> &str {
        "Insert_Never_Pattern_In_Guarded_Match_Arm_245"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = InsertNeverPatternVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions that have at least one arm with a guard. For each guarded match arm, it modifies the arm's pattern by adding an alternative that includes the never pattern `!`. If the existing pattern is a single pattern, it wraps it as `(pat | !)`. For compound patterns (like tuples or enum patterns), it inserts `!` as an additional alternative within the same pattern structure, ensuring syntactic validity. The guard and body remain unchanged. This transformation focuses on the interaction between never patterns and guards during MIR construction, potentially exposing edge cases in pattern exhaustiveness checking and guard evaluation."
    }
}

struct InsertNeverPatternVisitor;

impl VisitMut for InsertNeverPatternVisitor {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        for arm in &mut expr.arms {
            if let Some(_) = &arm.guard {
                if let Pat::TupleStruct(ts) = &mut arm.pat {
                    let mut alts = Punctuated::new();
                    alts.push_value(ts.clone());
                    alts.push_punct(Comma::default());
                    alts.push_value(parse_quote!(!));
                    arm.pat = parse_quote!((#alts));
                } else if let Pat::Tuple(t) = &mut arm.pat {
                    let mut alts = Punctuated::new();
                    alts.push_value(t.clone());
                    alts.push_punct(Comma::default());
                    alts.push_value(parse_quote!(!));
                    arm.pat = parse_quote!((#alts));
                } else if let Pat::Struct(s) = &mut arm.pat {
                    let mut alts = Punctuated::new();
                    alts.push_value(s.clone());
                    alts.push_punct(Comma::default());
                    alts.push_value(parse_quote!(!));
                    arm.pat = parse_quote!((#alts));
                } else if let Pat::Slice(s) = &mut arm.pat {
                    let mut alts = Punctuated::new();
                    alts.push_value(s.clone());
                    alts.push_punct(Comma::default());
                    alts.push_value(parse_quote!(!));
                    arm.pat = parse_quote!((#alts));
                } else if let Pat::Or(o) = &mut arm.pat {
                    o.leading_vert = None;
                    o.cases.push(parse_quote!(!));
                } else {
                    arm.pat = parse_quote!((#arm.pat | !));
                }
            }
        }
        visit_mut::visit_expr_match_mut(self, expr);
    }
}