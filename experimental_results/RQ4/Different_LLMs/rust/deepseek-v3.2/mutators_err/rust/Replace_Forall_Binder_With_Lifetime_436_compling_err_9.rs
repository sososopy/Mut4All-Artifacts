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

pub struct Replace_Forall_Binder_With_Lifetime_436;

impl Mutator for Replace_Forall_Binder_With_Lifetime_436 {
    fn name(&self) -> &str {
        "Replace_Forall_Binder_With_Lifetime_436"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ForallBinderReplacer::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates higher-ranked trait bounds using `for<T>` syntax and replaces them with concrete lifetime parameters. It first attempts to reuse existing lifetimes from the surrounding context; if none are available, it introduces a new lifetime parameter `'mut` into the generic parameter list. The bound is then transformed to use a type with that lifetime (e.g., `&'mut ()`). This transformation aims to expose compiler bugs in the handling of bound variables versus lifetime parameters under the non_lifetime_binders feature, potentially causing resolution errors or ICEs."
    }
}

struct ForallBinderReplacer {
    existing_lifetimes: HashSet<String>,
}

impl ForallBinderReplacer {
    fn new() -> Self {
        Self {
            existing_lifetimes: HashSet::new(),
        }
    }

    fn collect_lifetimes_from_generics(&mut self, generics: &Generics) {
        for param in &generics.params {
            if let GenericParam::Lifetime(lifetime_param) = param {
                self.existing_lifetimes.insert(lifetime_param.lifetime.ident.to_string());
            }
        }
    }

    fn choose_lifetime(&self) -> Lifetime {
        if let Some(existing) = self.existing_lifetimes.iter().next() {
            Lifetime::new(&format!("'{}", existing), Span::call_site())
        } else {
            Lifetime::new("'mut", Span::call_site())
        }
    }

    fn add_mut_lifetime_if_needed(&self, generics: &mut Generics) {
        if self.existing_lifetimes.is_empty() {
            generics.params.insert(0, parse_quote!('mut));
        }
    }
}

impl VisitMut for ForallBinderReplacer {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        self.collect_lifetimes_from_generics(&i.sig.generics);
        self.visit_generics_mut(&mut i.sig.generics);
        self.visit_block_mut(&mut i.block);
    }

    fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
        self.collect_lifetimes_from_generics(&i.generics);
        self.visit_generics_mut(&mut i.generics);
        for item in &mut i.items {
            self.visit_impl_item_mut(item);
        }
    }

    fn visit_generics_mut(&mut self, generics: &mut Generics) {
        if let Some(where_clause) = &mut generics.where_clause {
            self.visit_where_clause_mut(where_clause);
        }
        self.add_mut_lifetime_if_needed(generics);
    }

    fn visit_where_clause_mut(&mut self, where_clause: &mut WhereClause) {
        for predicate in &mut where_clause.predicates {
            if let WherePredicate::Type(predicate_type) = predicate {
                self.visit_type_mut(&mut predicate_type.bounded_ty);
                for bound in &mut predicate_type.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        self.visit_trait_bound_mut(trait_bound);
                    }
                }
            }
        }
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::TraitObject(trait_object) = ty {
            for bound in &mut trait_object.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    self.visit_trait_bound_mut(trait_bound);
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_trait_bound_mut(&mut self, trait_bound: &mut TraitBound) {
        if let Some(lifetimes) = &mut trait_bound.lifetimes {
            if !lifetimes.lifetimes.is_empty() {
                let chosen_lifetime = self.choose_lifetime();
                let mut new_path = trait_bound.path.clone();
                let last_segment = new_path.segments.last_mut().unwrap();
                match &mut last_segment.arguments {
                    PathArguments::AngleBracketed(args) => {
                        args.args = Punctuated::from_iter(vec![GenericArgument::Lifetime(chosen_lifetime)]);
                    }
                    PathArguments::Parenthesized(_) => {}
                    PathArguments::None => {
                        last_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                            colon2_token: None,
                            lt_token: token::Lt::default(),
                            args: Punctuated::from_iter(vec![GenericArgument::Lifetime(chosen_lifetime)]),
                            gt_token: token::Gt::default(),
                        });
                    }
                }
                trait_bound.lifetimes = None;
                trait_bound.path = new_path;
            }
        }
        syn::visit_mut::visit_trait_bound_mut(self, trait_bound);
    }
}