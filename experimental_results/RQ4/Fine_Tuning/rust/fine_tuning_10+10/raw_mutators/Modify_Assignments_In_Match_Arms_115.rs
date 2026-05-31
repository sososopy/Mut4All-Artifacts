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

pub struct Modify_Assignments_In_Match_Arms_115;

impl Mutator for Modify_Assignments_In_Match_Arms_115 {
    fn name(&self) -> &str {
        "Modify_Assignments_In_Match_Arms_115"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MatchVisitor;

        impl VisitMut for MatchVisitor {
            fn visit_expr_match_mut(&mut self, expr_match: &mut ExprMatch) {
                if let Some(Expr::Path(ExprPath { path, .. })) = expr_match.expr.as_ref() {
                    if path.segments.last().map_or(false, |seg| seg.ident == "Some" || seg.ident == "None") {
                        for arm in &mut expr_match.arms {
                            if let Pat::TupleStruct(pat_tuple_struct) = &arm.pat {
                                if pat_tuple_struct.path.segments.last().map_or(false, |seg| seg.ident == "Some") {
                                    if let Some(Pat::Ident(pat_ident)) = pat_tuple_struct.pat.elems.first() {
                                        let mut var_name = pat_ident.ident.clone();
                                        arm.body.stmts.insert(0, parse_quote! {
                                            #var_name = None;
                                        });
                                        arm.pat = parse_quote! { Some(mut #var_name) };
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_match_mut(self, expr_match);
            }
        }

        MatchVisitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions involving Option types. It identifies match arms handling Some(variable) and introduces a reassignment within this arm. The variable is reassigned to a new Option value, creating ambiguity in its usage, which can expose compiler bugs related to variable reassignments in match expressions."
    }
}