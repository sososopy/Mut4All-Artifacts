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

use std::collections::HashMap;

use crate::mutator::Mutator;

pub struct Modify_GAT_Lifetime_Constraints_In_Impl_130;

impl Mutator for Modify_GAT_Lifetime_Constraints_In_Impl_130 {
    fn name(&self) -> &str {
        "Modify_GAT_Lifetime_Constraints_In_Impl_130"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_map: HashMap<String, (usize, bool)> = HashMap::new();
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Trait(trait_item) = item {
                for item in &trait_item.items {
                    if let TraitItem::Type(type_item) = item {
                        if let Some(WhereClause { predicates, .. }) = &type_item.generics.where_clause {
                            for bound in predicates {
                                if let WherePredicate::Type(predicate_type) = bound {
                                    if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                        if type_path.path.is_ident(&type_item.ident) {
                                            trait_map.insert(
                                                trait_item.ident.to_string(),
                                                (index, true),
                                            );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = &impl_item.trait_ {
                    if let Some((trait_index, has_gat)) =
                        trait_map.get(&path.segments.last().unwrap().ident.to_string())
                    {
                        if !has_gat {
                            continue;
                        }
                        if let Item::Trait(trait_item) = &file.items[*trait_index] {
                            let mut gat_names = HashSet::new();
                            for item in &trait_item.items {
                                if let TraitItem::Type(type_item) = item {
                                    gat_names.insert(type_item.ident.to_string());
                                }
                            }
                            for item in &mut impl_item.items {
                                if let ImplItem::Type(type_item) = item {
                                    if gat_names.contains(&type_item.ident.to_string()) {
                                        if let Some(WhereClause { predicates, .. }) =
                                            &mut type_item.generics.where_clause
                                        {
                                            let mut new_bounds = Punctuated::new();
                                            for bound in predicates.iter() {
                                                if let WherePredicate::Type(predicate_type) = bound
                                                {
                                                    if let Type::Path(type_path) =
                                                        &predicate_type.bounded_ty
                                                    {
                                                        if type_path
                                                            .path
                                                            .is_ident(&type_item.ident)
                                                        {
                                                            let new_predicate: WherePredicate =
                                                                parse_quote!('b: 'a);
                                                            new_bounds.push(new_predicate);
                                                        } else {
                                                            new_bounds.push(bound.clone());
                                                        }
                                                    } else {
                                                        new_bounds.push(bound.clone());
                                                    }
                                                } else {
                                                    new_bounds.push(bound.clone());
                                                }
                                            }
                                            *predicates = new_bounds;
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
        "Identify traits with generic associated types (GATs) that have lifetime constraints. Locate implementations of these traits and modify the lifetime constraints in the associated type to introduce inconsistencies, such as reversing the order of lifetime bounds. This mutation targets the compiler's handling of GAT lifetime constraints, aiming to uncover bugs related to lifetime resolution and trait implementation validation."
    }
}