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

pub struct Add_Const_Trait_Bound_456;

impl Mutator for Add_Const_Trait_Bound_456 {
    fn name(&self) -> &str {
        "Add_Const_Trait_Bound_456"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_traits = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                existing_traits.push(trait_item.ident.clone());
            }
        }
        if existing_traits.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some(const_token) = &item_impl.constness {
                    let trait_path = &item_impl.trait_;
                    if let Some((_, trait_ref, _)) = trait_path {
                        let trait_name = trait_ref.segments.last().map(|seg| &seg.ident);
                        if trait_name.is_some() && !existing_traits.is_empty() {
                            let target_trait = existing_traits.choose(&mut rng).unwrap();
                            if let Some(generics) = &mut item_impl.generics {
                                if !generics.params.is_empty() {
                                    let where_clause = generics.where_clause.get_or_insert_with(|| WhereClause {
                                        where_token: Default::default(),
                                        predicates: Punctuated::new(),
                                    });
                                    let new_predicate: WherePredicate = parse_quote! {
                                        T: const #target_trait
                                    };
                                    where_clause.predicates.push(new_predicate);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations that use the const keyword. It adds a const trait bound to a generic parameter in the where clause, referencing an existing trait from the seed program. This creates overlapping const trait constraints that stress the compiler's specialization and const trait resolution logic, potentially triggering ICEs similar to the bug report."
    }
}