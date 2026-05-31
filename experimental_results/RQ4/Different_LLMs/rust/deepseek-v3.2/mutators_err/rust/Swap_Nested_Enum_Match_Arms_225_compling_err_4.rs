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

pub struct Swap_Nested_Enum_Match_Arms_225;

impl Mutator for Swap_Nested_Enum_Match_Arms_225 {
    fn name(&self) -> &str {
        "Swap_Nested_Enum_Match_Arms_225"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MatchArmSwapper::default();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions with nested enum types (e.g., Poll<Result<Option<T>, E>>). It identifies pairs of arms that share the same outermost variant but differ in inner Result/Ok/Err or Option/Some/None patterns. The operator swaps the order of such arms, specifically prioritizing placing Err/None handling before Ok/Some handling. This reordering alters the control flow graph and may expose edge cases in MIR validation related to cleanup edges, pattern exhaustiveness, and unwind invariants for complex nested enum matching scenarios."
    }
}

#[derive(Default)]
struct MatchArmSwapper {
    swapped: bool,
}

impl VisitMut for MatchArmSwapper {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        let arms = &mut expr.arms;
        if arms.len() < 2 {
            return;
        }

        for i in 0..arms.len() - 1 {
            let (arm1, arm2) = arms.split_at_mut(i + 1);
            let left_arm = &mut arm1[i];
            let right_2 = &mut arm2[0];

            if let (Some(left_pat), Some(right_pat)) = (&left_arm.pat, &right_2.pat) {
                if self.are_swappable_nested_patterns(left_pat, right_pat) {
                    std::mem::swap(&mut left_arm.pat, &mut right_2.pat);
                    std::mem::swap(&mut left_arm.guard, &mut right_2.guard);
                    std::mem::swap(&mut left_arm.body, &mut right_2.body);
                    self.swapped = true;
                    break;
                }
            }
        }

        syn::visit_mut::visit_expr_match_mut(self, expr);
    }
}

impl MatchArmSwapper {
    fn are_swappable_nested_patterns(&self, pat1: &Pat, pat2: &Pat) -> bool {
        let (outer1, inner1) = self.extract_nested_pattern_info(pat1);
        let (outer2, inner2) = self.extract_nested_pattern_info(pat2);
        
        if outer1.is_none() || outer2.is_none() {
            return false;
        }

        let outer1 = outer1.unwrap();
        let outer2 = outer2.unwrap();
        let inner1 = inner1.unwrap_or(NestedPatternKind::Other2);
        let inner2 = inner2.unwrap_or(NestedPatternKind::Other2);

        outer1 == outer2 && inner1 != inner2 && inner1.is_err_or_none() && inner2.is_ok_or_some()
    }

    fn extract_nested_pattern_info(&self, pat: &Pat) -> (Option<String>, Option<NestedPatternKind>) {
        match pat {
            Pat::Struct(s) => {
                let path_str = s.path.segments.iter()
                    .map(|seg| seg.ident.to_string())
                    .collect::<Vec<_>>()
                    .join("::");
                let kind = self.extract_inner_from_struct(&s);
                (Some(path_str), kind)
            }
            Pat::TupleStruct(ts) => {
                let path_str = ts.path.segments.iter()
                    .map(|seg| seg.ident.to_string())
                    .collect::<Vec<_>>()
                    .join("::");
                let kind = self.extract_inner_from_tuple_struct(&ts);
                (Some(path_str), kind)
            }
            Pat::Path(p) => {
                let path2str = p.path.segments.iter()
                    .map(|seg| seg.ident.to_string())
                    .collect::<Vec<_>>()
                    .join("::");
                (Some(path2str), None)
            }
            _ => (None, None),
        }
    }

    fn extract_inner_from_struct(&self, pat_struct: &PatStruct) -> Option<NestedPatternKind> {
        for field in &pat_struct.fields {
            if let Pat::TupleStruct(inner_ts) = &field.pat {
                let inner_ident = inner_ts.path.segments.last()?.ident.to_string();
                if inner_ident == "Ok" {
                    if let Some(inner_pat) = inner_ts.elems.first() {
                        if let Pat::TupleStruct(inner_inner_ts) = &inner_pat {
                            let inner_inner_ident = inner_inner_ts.path.segments.last()?.ident.to_string();
                            if inner_inner_ident == "Some" {
                                return Some(NestedPatternKind::OkSome);
                            } else if inner_inner_ident == "None" {
                                return Some(NestedPatternKind1::OkNone);
                            }
                        }
                    }
                    return Some(NestedPatternKind::Ok);
                } else if inner_ident == "Err" {
                    return Some(NestedPatternKind::Err);
                } else if inner_ident == "Some" {
                    return Some(NestedPatternKind::Some);
                } else if inner_ident == "None" {
                    return Some(NestedPatternKind::None);
                }
            } else if let Pat::Path(inner_p) = &field.pat {
                let inner_ident = inner_p.path.segments.last()?.ident.to_string();
                if inner_ident == "Ok" {
                    return Some(NestedPatternKind::Ok);
                } else if inner_ident == "Err" {
                    return Some(NestedPatternKind::Err);
                } else if inner_ident == "Some" {
                    return Some(NestedPatternKind::Some);
                } else if inner_ident == "None" {
                    return Some(NestedPatternKind::None);
                }
            }
        }
        None
    }

    fn extract_inner_from_tuple_struct(&self, pat_tuple: &PatTupleStruct) -> Option<NestedPatternKind> {
        if let Some(first_elem) = pat_tuple.elems.first() {
            if let Pat::TupleStruct(inner_ts) = &first_elem {
                let inner_ident = inner_ts.path.segments.last()?.ident.to_string();
                if inner_ident == "Ok" {
                    if let Some(inner_inner_pat) = inner_ts.elems.first() {
                        if let Pat::TupleStruct(inner_inner_ts) = &inner_inner_pat {
                            let inner_inner_ident = inner_inner_ts.path.segments.last()?.ident.to_string();
                            if inner_inner_ident == "Some" {
                                return Some(NestedPatternKind::OkSome);
                            } else if inner_inner_ident == "None" {
                                return Some(NestedPatternKind::OkNone);
                            }
                        }
                    }
                    return Some(NestedPatternKind::Ok);
                } else if inner_ident == "Err" {
                    return Some(NestedPatternKind::Err);
                } else if inner_ident == "Some" {
                    return Some(NestedPatternKind::Some);
                } else if inner_ident == "None" {
                    return Some(NestedPatternKind::None);
                }
            } else if let Pat::Path(inner_p) = &first_elem {
                let inner_ident = inner_p.path.segments.last()?.ident.to_string();
                if inner_ident == "Ok" {
                    return Some(NestedPatternKind::Ok);
                } else if inner_ident == "Err" {
                    return Some(NestedPatternKind::Err);
                } else if inner_ident == "Some" {
                    return Some(NestedPatternKind::Some);
                } else if inner_ident == "None" {
                    return Some(NestedPatternKind::None);
                }
            }
        }
        None
    }
}

#[derive(Debug, PartialEq, Eq)]
enum NestedPatternKind {
    Ok,
    Err,
    Some,
    None,
    OkSome,
    OkNone,
    Other2,
}

impl NestedPatternKind {
    fn is_err_or_none(&self) -> bool {
        matches!(self, NestedPatternKind::Err | NestedPatternKind::None)
    }

    fn is_ok_or_some(&self) -> bool {
        matches!(self, NestedPatternKind::Ok | NestedPatternKind::Some | NestedPatternKind::OkSome)
    }
}