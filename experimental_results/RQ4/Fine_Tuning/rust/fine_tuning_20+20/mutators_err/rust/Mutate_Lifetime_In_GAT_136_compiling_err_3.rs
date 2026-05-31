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

pub struct Mutate_Lifetime_In_GAT_136;

impl Mutator for Mutate_Lifetime_In_GAT_136 {
    fn name(&self) -> &str {
        "Mutate_Lifetime_In_GAT_136"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let TraitItem::Type(ty_item) = item {
                        if let Some(where_clause) = &mut ty_item.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                        if type_path.path.is_ident("Self") {
                                            predicate_type.bounded_ty =
                                                parse_quote!('a);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits with generic associated types (GATs) that have lifetime constraints on `Self`. It transforms the constraint from `Self: 'a` to `'a: 'static`, creating a logically incorrect lifetime bound. This mutation is designed to stress-test the compiler's lifetime resolution and constraint checking mechanism, particularly in the context of GATs. It can potentially expose ICEs (Internal Compiler Errors) related to lifetime mismanagement or incorrect bound inference."
    }
}