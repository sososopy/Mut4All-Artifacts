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

pub struct Replace_For_Const_Binder_With_Constant_109;

impl Mutator for Replace_For_Const_Binder_With_Constant_109 {
    fn name(&self) -> &str {
        "Replace_For_Const_Binder_With_Constant_109"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut constants = HashSet::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                constants.insert(item_const.ident.to_string());
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            if let Type::Array(type_array) = &pred_type.bounded_ty {
                                let mut visitor = ConstBinderVisitor {
                                    constants: &constants,
                                    changed: false,
                                };
                                visitor.visit_type_mut(&mut pred_type.bounded_ty);
                                if visitor.changed {
                                    pred_type.lifetimes = None;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets where-clauses containing `for<const C: T>` binders that introduce late-bound const generic parameters used in array types like `[(); C]`. It scans for existing constants in the1 program and replaces the binder with a concrete constant expression (either a named constant or the literal `0`), removing the `for<const ...>` binder entirely. This transformation converts late-bound parameters to early-bound constants, testing the compiler's handling of const generic bounds and potentially triggering bugs in const evaluation and trait resolution."
    }
}

struct ConstBinderVisitor<'a> {
    constants: &'a HashSet<String>,
    changed: bool,
}

impl<'a> VisitMut for ConstBinderVisitor<'a> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Array(type_array) = node {
            if let Expr::Path(expr_path) = &type_array.len {
                if expr_path.path.segments.len() == 1 {
                    let ident = expr_path.path.segments[0].ident.to_string();
                    if self.constants.contains(&ident) {
                        return;
                    }
                }
            }
            let replacement = if self.constants.is_empty() {
                parse_quote!(0)
            } else {
                let constant = self.constants.iter().next().unwrap();
                parse_quote!(#constant)
            };
            type_array.len = replacement;
            self.changed = true;
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}