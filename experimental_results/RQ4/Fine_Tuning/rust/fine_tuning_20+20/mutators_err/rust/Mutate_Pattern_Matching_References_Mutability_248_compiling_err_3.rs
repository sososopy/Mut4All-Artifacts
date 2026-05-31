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

pub struct Mutate_Pattern_Matching_References_Mutability_248;

impl Mutator for Mutate_Pattern_Matching_References_Mutability_248 {
    fn name(&self) -> &str {
        "Mutate_Pattern_Matching_References_Mutability_248"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct PatternMatcher<'a> {
            patterns: &'a mut Vec<Pat>,
        }

        impl<'a> Visit<'a> for PatternMatcher<'a> {
            fn visit_pat(&mut self, node: &'a Pat) {
                if matches!(node, Pat::Reference(_)) {
                    self.patterns.push(node.clone());
                }
                syn::visit::visit_pat(self, node);
            }
        }

        struct ReferenceExprCollector<'a> {
            exprs: &'a mut Vec<Expr>,
        }

        impl<'a> Visit<'a> for ReferenceExprCollector<'a> {
            fn visit_expr(&mut self, node: &'a Expr) {
                if matches!(node, Expr::Reference(_)) {
                    self.exprs.push(node.clone());
                }
                syn::visit::visit_expr(self, node);
            }
        }

        struct ReferenceExprFinder {
            exprs: Vec<Expr>,
        }

        impl ReferenceExprFinder {
            fn find_in_expr(&mut self, expr: &Expr) {
                let mut collector = ReferenceExprCollector {
                    exprs: &mut self.exprs,
                };
                collector.visit_expr(expr);
            }
        }

        fn find_reference_exprs(expr: &Expr) -> Vec<Expr> {
            let mut finder = ReferenceExprFinder { exprs: Vec::new() };
            finder.find_in_expr(expr);
            finder.exprs
        }

        fn is_mutable_reference(expr: &Expr) -> bool {
            matches!(
                expr,
                Expr::Reference(ExprReference {
                    mutability: Some(_),
                    ..
                })
            )
        }

        fn is_immutable_reference(expr: &Expr) -> bool {
            matches!(
                expr,
                Expr::Reference(ExprReference {
                    mutability: None,
                    ..
                })
            )
        }

        fn has_mutable_reference_pattern(patterns: &[Pat]) -> bool {
            patterns.iter().any(|pat| {
                matches!(
                    pat,
                    Pat::Reference(PatReference {
                        mutability: Some(_),
                        ..
                    })
                )
            })
        }

        fn has_immutable_reference_pattern(patterns: &[Pat]) -> bool {
            patterns.iter().any(|pat| {
                matches!(
                    pat,
                    Pat::Reference(PatReference {
                        mutability: None,
                        ..
                    })
                )
            })
        }

        fn replace_reference_with_mutability(expr: &mut Expr, mutability: bool) {
            if let Expr::Reference(ref_expr) = expr {
                ref_expr.mutability = if mutability {
                    Some(token::Mut {
                        span: Span::call_site(),
                    })
                } else {
                    None
                };
            }
        }

        fn replace_pattern_with_mutability(pat: &mut Pat, mutability: bool) {
            if let Pat::Reference(ref_pat) = pat {
                ref_pat.mutability = if mutability {
                    Some(token::Mut {
                        span: Span::call_site(),
                    })
                } else {
                    None
                };
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_mutated = false;
                for stmt in &mut item_fn.block.stmts {
                    if has_mutated {
                        break;
                    }
                    if let Stmt::Expr(expr, _) | Stmt::Semi(expr, _) = stmt {
                        if let Expr::If(expr_if) = expr {
                            if let Expr::Let(expr_let) = &*expr_if.cond {
                                let mut patterns = Vec::new();
                                let mut matcher = PatternMatcher {
                                    patterns: &mut patterns,
                                };
                                matcher.visit_pat(&expr_let.pat);
                                if patterns.len() <= 1 {
                                    continue;
                                }
                                let mut reference_exprs = find_reference_exprs(&expr_let.expr);
                                if reference_exprs.len() <= 1 {
                                    continue;
                                }
                                if has_mutable_reference_pattern(&patterns)
                                    && reference_exprs
                                        .iter()
                                        .any(|expr| is_immutable_reference(expr))
                                {
                                    for pattern in &mut patterns {
                                        replace_pattern_with_mutability(pattern, false);
                                    }
                                    for expr in &mut reference_exprs {
                                        replace_reference_with_mutability(expr, false);
                                    }
                                    has_mutated = true;
                                } else if has_immutable_reference_pattern(&patterns)
                                    && reference_exprs
                                        .iter()
                                        .any(|expr| is_mutable_reference(expr))
                                {
                                    for pattern in &mut patterns {
                                        replace_pattern_with_mutability(pattern, true);
                                    }
                                    for expr in &mut reference_exprs {
                                        replace_reference_with_mutability(expr, true);
                                    }
                                    has_mutated = true;
                                }
                            }
                        } else if let Expr::Match(expr_match) = expr {
                            let mut arm_idx = 0usize;
                            while arm_idx < expr_match.arms.len() {
                                if has_mutated {
                                    break;
                                }
                                let arm = &mut expr_match.arms[arm_idx];
                                let mut patterns = Vec::new();
                                let mut matcher = PatternMatcher {
                                    patterns: &mut patterns,
                                };
                                matcher.visit_pat(&arm.pat);
                                if patterns.len() <= 1 {
                                    arm_idx += 1;
                                    continue;
                                }
                                let mut reference_exprs = find_reference_exprs(&expr_match.expr);
                                if reference_exprs.len() <= 1 {
                                    arm_idx += 1;
                                    continue;
                                }
                                if has_mutable_reference_pattern(&patterns)
                                    && reference_exprs
                                        .iter()
                                        .any(|expr| is_immutable_reference(expr))
                                {
                                    for pattern in &mut patterns {
                                        replace_pattern_with_mutability(pattern, false);
                                    }
                                    for expr in &mut reference_exprs {
                                        replace_reference_with_mutability(expr, false);
                                    }
                                    has_mutated = true;
                                } else if has_immutable_reference_pattern(&patterns)
                                    && reference_exprs
                                        .iter()
                                        .any(|expr| is_mutable_reference(expr))
                                {
                                    for pattern in &mut patterns {
                                        replace_pattern_with_mutability(pattern, true);
                                    }
                                    for expr in &mut reference_exprs {
                                        replace_reference_with_mutability(expr, true);
                                    }
                                    has_mutated = true;
                                }
                                arm_idx += 1;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets pattern matching constructs (`if let` and `match`) that involve both reference and mutable reference patterns. It identifies cases where a pattern matches against a reference and the corresponding value is also a reference. The mutator then swaps the mutability of the reference pattern and the matched value, either converting `&` to `&mut` or vice versa. This transformation stresses the compiler's handling of reference mutability in pattern matching, particularly in nested or complex match scenarios."
    }
}