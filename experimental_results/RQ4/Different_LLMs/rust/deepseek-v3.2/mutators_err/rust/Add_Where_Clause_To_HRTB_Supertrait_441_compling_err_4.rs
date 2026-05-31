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

pub struct Add_Where_Clause_To_HRTB_Supertrait_441;

impl Mutator for Add_Where_Clause_To_HRTB_Supertrait_441 {
    fn name(&self) -> &str {
        "Add_Where_Clause_To_HRTB_Supertrait_441"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut other_traits = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                other_traits.push(trait_item.ident.clone());
            }
        }
        let dummy_trait_name = Ident::new("DummyTraitForHRTB", Span::call_site());
        let mut has_dummy = false;
        if other_traits.is_empty() {
            let dummy_trait: Item = parse_quote! {
                trait DummyTraitForHRTB {}
            };
            file.items.insert(0, dummy_trait);
            has_dummy = true;
            other_traits.push(dummy_trait_name.clone());
        }
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut new_bounds = Punctuated::new();
                for bound in &trait_item.supertraits {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(hr_trait_bound) = extract_hr_trait_bound(trait_bound) {
                            let mut new_trait_bound = trait_bound.clone();
                            let target_trait = other_traits.choose(&mut thread_rng()).unwrap().clone();
                            let where_clause: WhereClause = parse_quote! {
                                where T: #target_trait
                            };
                            if let PathArguments::AngleBracketed(angle_args) = &mut new_trait_bound.path.segments.last_mut().unwrap().arguments {
                                angle_args.colon2_token = None;
                                if let Some(predicate) = where_clause.predicates.first() {
                                    let generic_arg: GenericArgument = parse_quote!(#predicate);
                                    angle_args.args.push(generic_arg);
                                }
                            }
                            new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                        } else {
                            new_bounds.push(bound.clone());
                        }
                    } else {
                        new_bounds.push(bound.clone());
                    }
                }
                trait_item.supertraits = new_bounds;
            }
        }
        if has_dummy {
            let mut index_to_remove = None;
            for (i, item) in file.items.iter().enumerate() {
                if let Item::Trait(trait_item) = item {
                    if trait_item.ident == dummy_trait_name {
                        index_to_remove = Some(i);
                        break;
                    }
                }
            }
            if let Some(index) = index_to_remove {
                file.items.remove(index);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions with higher-ranked trait bounds (HRTB) as supertraits. It modifies each HRTB supertrait by adding a where clause that constrains the non-lifetime parameter with another trait from the seed program (or a newly introduced dummy trait). This transformation stresses the compiler's handling of additional constraints in HRTBs with non-lifetime parameters during trait object formation and method selection, potentially triggering internal compiler errors related to the non_lifetime_binders feature."
    }
}

fn extract_hr_trait_bound(trait_bound: &TraitBound) -> Option<&TraitBound> {
    if trait_bound.lifetimes.is_none() {
        return None;
    }
    if let Some(BoundLifetimes { lifetimes, .. }) = &trait_bound.lifetimes {
        if lifetimes.len() == 1 {
            if let GenericParam::Lifetime(LifetimeParam { lifetime, .. }) = &lifetimes[0] {
                if lifetime.ident == "T" {
                    return Some(trait_bound);
                }
            }
        }
    }
    None
}