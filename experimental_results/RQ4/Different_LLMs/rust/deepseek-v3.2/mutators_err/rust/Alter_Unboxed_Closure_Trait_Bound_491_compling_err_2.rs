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

pub struct Alter_Unboxed_Closure_Trait_Bound_491;

impl Mutator for Alter_Unboxed_Closure_Trait_Bound_491 {
    fn name(&self) -> &str {
        "Alter_Unboxed_Closure_Trait_Bound_491"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureTraitBoundVisitor {
            rng: thread_rng(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closure trait bounds (Fn, FnMut, FnOnce) in generic contexts, including where clauses, trait bounds, and dyn trait objects. It alters the closure trait to a different one (cycling through the three) while preserving generic arguments and associated types. This transformation tests the compiler's robustness in handling different closure traits in advanced scenarios like unboxed closures, associated types, and dynamic dispatch, potentially triggering ICEs during MIR inlining or trait resolution."
    }
}

struct ClosureTraitBoundVisitor {
    rng: rand::rngs::ThreadRng,
}

impl ClosureTraitBoundVisitor {
    fn alter_trait_bound(&mut self, bound: &mut TypeParamBound) {
        if let TypeParamBound::Trait(trait_bound) = bound {
            let path = &mut trait_bound.path;
            if let Some(segment) = path.segments.last_mut() {
                let ident_str = segment.ident.to_string();
                if ident_str == "Fn" || ident_str == "FnMut" || ident_str == "FnOnce" {
                    let alternatives = ["Fn", "FnMut", "FnOnce"];
                    let current_index = alternatives.iter().position(|&x| x == ident_str).unwrap();
                    let next_index = (current_index + 1) % alternatives.len();
                    segment.ident = Ident::new(alternatives[next_index], segment.ident.span());
                }
            }
        }
    }

    fn alter_trait_object(&mut self, ty: &mut Type) {
        if let Type::TraitObject(trait_obj) = ty {
            for bound in &mut trait_obj.bounds {
                self.alter_trait_bound(bound);
            }
        }
    }

    fn alter_where_predicate(&mut self, predicate: &mut WherePredicate) {
        match predicate {
            WherePredicate::Type(pred_type) => {
                for bound in &mut pred_type.bounds {
                    self.alter_trait_bound(bound);
                }
            }
            WherePredicate::Lifetime(_) => {}
            _ => {}
        }
    }
}

impl VisitMut for ClosureTraitBoundVisitor {
    fn visit_generic_param_mut(&mut self, param: &mut GenericParam) {
        if let GenericParam::Type(type_param) = param {
            for bound in &mut type_param.bounds {
                self.alter_trait_bound(bound);
            }
        }
        syn::visit_mut::visit_generic_param_mut(self, param);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        self.alter_trait_object(ty);
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_where_clause_mut(&mut self, where_clause: &mut WhereClause) {
        for predicate in &mut where_clause.predicates {
            self.alter_where_predicate(predicate);
        }
        syn::visit_mut::visit_where_clause_mut(self, where_clause);
    }

    fn visit_trait_bound_mut(&mut self, bound: &mut TraitBound) {
        let path = &mut bound.path;
        if let Some(segment) = path.segments.last_mut() {
            let ident_str = segment.ident.to_string();
            if ident_str == "Fn" || ident_str == "FnMut" || ident_str == "FnOnce" {
                let alternatives = ["Fn", "FnMut", "FnOnce"];
                let current_index = alternatives.iter().position(|&x| x == ident_str).unwrap();
                let next_index = (current_index + 1) % alternatives.len();
                segment.ident = Ident::new(alternatives[next_index], segment.ident.span());
            }
        }
        syn::visit_mut::visit_trait_bound_mut(self, bound);
    }
}