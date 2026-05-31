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

pub struct Toggle_Const_Trait_Bound_312;

impl Mutator for Toggle_Const_Trait_Bound_312 {
    fn name(&self) -> &str {
        "Toggle_Const_Trait_Bound_312"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstTraitBoundVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator toggles the `~const` modifier on trait bounds in where clauses, specifically targeting `Self: ~const Trait` bounds. It removes `~const` from existing const bounds or adds it to regular bounds when in a const context (detected via `#[const_trait]` attribute or const impls). This transformation tests the compiler's handling of const trait bound consistency, potentially exposing internal compiler errors during lowering phases where missing host param ids might be encountered."
    }
}

struct ConstTraitBoundVisitor;

impl VisitMut for ConstTraitBoundVisitor {
    fn visit_item_trait_mut(&mut self, node: &mut ItemTrait) {
        let has_const_attr = node.attrs.iter().any(|attr| {
            attr.path().is_ident("const_trait")
        });
        self.visit_generics_mut(&mut node.generics, has_const_attr);
        for item in &mut node.items {
            if let TraitItem::Fn(method) = item {
                self.visit_generics_mut(&mut method.sig.generics, has_const_attr);
            }
        }
        visit_mut::visit_item_trait_mut(self, node);
    }

    fn visit_item_impl_mut(&mut self, node: &mut ItemImpl) {
        let is_const_impl = node.trait_.is_some() && node.items.iter().any(|item| {
            if let ImplItem::Fn(method) = item {
                method.sig.constness.is_some()
            } else {
                false
            }
        });
        self.visit_generics_mut(&mut node.generics, is_const_impl);
        for item in &mut node.items {
            if let ImplItem::Fn(method) = item {
                self.visit_generics_mut(&mut method.sig.generics, is_const_impl);
            }
        }
        visit_mut::visit_item_impl_mut(self, node);
    }
}

impl ConstTraitBoundVisitor {
    fn visit_generics_mut(&mut self, generics: &mut Generics, is_const_context: bool) {
        if let Some(where_clause) = &mut generics.where_clause {
            for predicate in &mut where_clause.predicates {
                if let WherePredicate::Type(pred_type) = predicate {
                    self.toggle_const_on_bounds(&mut pred_type.bounds, is_const_context);
                }
            }
        }
    }

    fn toggle_const_on_bounds(&self, bounds: &mut Punctuated<TypeParamBound, token::Plus>, is_const_context: bool) {
        for bound in bounds.iter_mut() {
            if let TypeParamBound::Trait(trait_bound) = bound {
                if let Some(path) = trait_bound.path.segments.last() {
                    if path.ident == "Self" {
                        continue;
                    }
                }
                match trait_bound.modifier {
                    TraitBoundModifier::MaybeConst => {
                        if !is_const_context {
                            trait_bound.modifier = TraitBoundModifier::None;
                        }
                    }
                    TraitBoundModifier::None => {
                        if is_const_context {
                            trait_bound.modifier = TraitBoundModifier::MaybeConst;
                        }
                    }
                    _ => {}
                }
            }
        }
    }
}