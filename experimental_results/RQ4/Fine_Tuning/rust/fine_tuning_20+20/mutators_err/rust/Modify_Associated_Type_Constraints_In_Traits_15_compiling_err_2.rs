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

pub struct Modify_Associated_Type_Constraints_In_Traits_15;

impl Mutator for Modify_Associated_Type_Constraints_In_Traits_15 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_Constraints_In_Traits_15"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut trait_names = Vec::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                trait_names.push(item_trait.ident.to_string());
            }
        }
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Type(type_item) = item {
                        let bounds = &mut type_item.bounds;
                        if !trait_names.is_empty() {
                            let random_trait =
                                trait_names.choose(&mut rng).unwrap().to_string();
                            let new_bound: TypeParamBound = parse_quote!(#random_trait);
                            bounds.push(new_bound);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets associated types within traits that have constraints. It randomly selects an additional trait from the available traits in the seed program and adds it as a new bound to the associated type. This modification increases the complexity of trait constraints and tests the compiler's ability to handle multiple trait bounds on associated types, potentially exposing issues in trait resolution and normalization."
    }
}