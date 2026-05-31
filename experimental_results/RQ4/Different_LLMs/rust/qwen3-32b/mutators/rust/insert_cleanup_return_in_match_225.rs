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

pub struct Insert_Cleanup_Return_In_Match_225;

impl Mutator for Insert_Cleanup_Return_In_Match_225 {
    fn name(&self) -> &str {
        "Insert_Cleanup_Return_In_Match_225"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MatchVisitor;
        impl VisitMut for MatchVisitor {
            fn visit_expr_match_mut(&mut self, expr_match: &mut ExprMatch) {
                for arm in &mut expr_match.arms {
                    if let Some(var) = find_binding_var(&arm.pat) {
                        let new_body = parse_quote! {{
                            drop(#var);
                            return;
                        }};
                        arm.body = Box::new(new_body);
                    }
                }
                syn::visit_mut::visit_expr_match_mut(self, expr_match);
            }
        }
        let mut visitor = MatchVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a cleanup block followed by a return in match arms over Result/Poll variants. It identifies bound variables in match patterns and inserts a drop statement for the variable, followed by an immediate return. This mimics unwind safety violations and tests how the compiler handles resource cleanup and control flow invariants."
    }
}

fn find_binding_var(pat: &Pat) -> Option<Ident> {
    match pat {
        Pat::Ident(pat_ident) => Some(pat_ident.ident.clone()),
        Pat::Wild(_) => None,
        Pat::Struct(pat_struct) => {
            for field in &pat_struct.fields {
                if let Some(var) = find_binding_var(&field.pat) {
                    return Some(var);
                }
            }
            None
        },
        Pat::TupleStruct(pat_tuple_struct) => {
            for pat in &pat_tuple_struct.elems {
                if let Some(var) = find_binding_var(pat) {
                    return Some(var);
                }
            }
            None
        },
        Pat::Tuple(pat_tuple) => {
            for pat in &pat_tuple.elems {
                if let Some(var) = find_binding_var(pat) {
                    return Some(var);
                }
            }
            None
        },
        Pat::Or(pat_or) => {
            for pat in &pat_or.cases {
                if let Some(var) = find_binding_var(pat) {
                    return Some(var);
                }
            }
            None
        },
        _ => None,
    }
}