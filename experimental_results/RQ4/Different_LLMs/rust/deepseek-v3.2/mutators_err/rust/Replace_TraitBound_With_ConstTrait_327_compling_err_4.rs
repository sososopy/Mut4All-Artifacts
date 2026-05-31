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

pub struct Replace_TraitBound_With_ConstTrait_327;

impl Mutator for Replace_TraitBound_With_ConstT1rait_327 {
    fn name(&self) -> &str {
        "Replace_TraitBound_With_ConstTrait_327"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_trait_set = HashSet::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                for attr in &trait_item.attrs {
                    if attr.path().is_ident("const_trait") {
                        const_trait_set.insert(trait_item.ident.to_string());
                        break;
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let TraitItem::Type(assoc_type) = item {
                        let mut new_bounds = Punctuated::new();
                        for bound in &assoc_type.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let path_segments = &trait_bound.path.segments;
                                if let Some(last_seg) = path_segments.last() {
                                    if const_trait_set.contains(&last_seg.ident.to_string()) {
                                        let mut new_trait_bound = trait_bound.clone();
                                        new_trait_bound.modifier = TraitBoundModifier::Const;
                                        new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                        continue;
                                    }
                                }
                            }
                            new_bounds.push(bound.clone());
                        }
                        assoc_type.bounds = new_bounds;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator scans for traits marked with #[const_trait] attribute, collects their names, then traverses all trait definitions to find associated type bounds. When an associated type has a bound referencing one of those const traits, it modifies the bound to become a const trait bound (e.g., ': Trait' becomes ': const Trait'). This transformation targets the compiler's handling of const trait bounds in implied bounds calculations, potentially exposing bugs in const trait bound resolution and associated type projection."
    }
}