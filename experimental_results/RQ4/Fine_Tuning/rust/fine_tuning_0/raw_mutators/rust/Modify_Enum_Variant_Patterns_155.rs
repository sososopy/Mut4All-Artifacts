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

pub struct Modify_Enum_Variant_Patterns_155;

impl Mutator for Modify_Enum_Variant_Patterns_155 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_Patterns_155"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = MatchVisitor;
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum variant pattern matching in match expressions. It replaces specific patterns with variable bindings, ensuring the variable is used within the match arm. This transformation tests the compiler's handling of pattern matching, variable binding, and type inference in the presence of enums."
    }
}

struct MatchVisitor;

impl VisitMut for MatchVisitor {
    fn visit_expr_match_mut(&mut self, expr_match: &mut ExprMatch) {
        for arm in &mut expr_match.arms {
            if let Pat::Path(pat_path) = &arm.pat {
                if let Some(segment) = pat_path.path.segments.last() {
                    let ident = &segment.ident;
                    let new_ident = Ident::new(&format!("Var_{}", ident), Span::call_site());
                    let new_pat: Pat = parse_quote! { #new_ident(y) };
                    arm.pat = new_pat;
                    let new_stmt: Stmt = parse_quote! { process(y); };
                    arm.body = Box::new(parse_quote! { { #new_stmt } });
                    break;
                }
            }
        }
        visit_mut::visit_expr_match_mut(self, expr_match);
    }
}