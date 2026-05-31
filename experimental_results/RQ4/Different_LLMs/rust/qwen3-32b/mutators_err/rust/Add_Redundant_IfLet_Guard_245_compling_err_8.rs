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

pub struct Add_Redundant_IfLet_Guard_245;

impl Mutator for Add_Redundant_IfLet_Guard_245 {
    fn name(&self) -> &str {
        "Add_Redundant_IfLet_Guard_245"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MatchArmVisitor;
        impl VisitMut for MatchArmVisitor {
            fn visit_expr_match_mut(&mut self, expr: &mut syn::ExprMatch) {
                for arm in &mut expr.arms {
                    if let Some(guard) = arm.guard.as_mut() {
                        if !arm.pat.is_refutable() {
                            *guard = parse_quote! { if (&[]).split_last().is_some() };
                        }
                    }
                }
            }
        }
        let mut visitor = MatchArmVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a redundant if let guard to match arms with irrefutable or never patterns that already have a guard. The guard uses an Option-returning function call (e.g., split_last(&())) to create a syntactically valid but semantically redundant condition. This tests the compiler's pattern guard validation and MIR construction logic under complex guard interactions."
    }
}