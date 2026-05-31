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

pub struct Replace_Match_Arm_Pattern_With_Undeclared_Variable_202;

impl Mutator for Replace_Match_Arm_Pattern_With_Undeclared_Variable_202 {
    fn name(&self) -> &str {
        "Replace_Match_Arm_Pattern_With_Undeclared_Variable_202"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MatchVisitor;
        impl<'ast> VisitMut for MatchVisitor {
            fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
                if expr.arms.len() >= 2 {
                    for arm in &mut expr.arms {
                        match &arm.pat {
                            Pat::Ident(_) | Pat::Wild(_) => continue,
                            _ => {
                                arm.pat = *create_undeclared_variable_pattern();
                                break;
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_match_mut(self, expr);
            }
        }
        let mut visitor = MatchVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a match arm pattern with an undeclared variable pattern (FOO_REF_REF), targeting match expressions with at least two arms and compound/range patterns. This introduces an invalid pattern, forcing the compiler's late lint to detect the missing variable declaration, potentially exposing bugs in pattern validation and lint infrastructure."
    }
}

fn create_undeclared_variable_pattern() -> Box<syn::Pat> {
    let ident = Ident::new("FOO_REF_REF", Span::call_site());
    Box::new(Pat::Ident(PatIdent {
        by_ref: None,
        mutability: None,
        ident,
        subpat: None,
    }))
}